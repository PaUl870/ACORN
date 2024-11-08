#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <cstring>


#include <sys/time.h>

#include <faiss/IndexFlat.h>
#include <faiss/IndexHNSW.h>
#include <faiss/IndexACORN.h>
#include <faiss/index_io.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


// added these
#include <faiss/Index.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <iostream>
#include <sstream>      // for ostringstream
#include <fstream>  
#include <iosfwd>
#include <faiss/impl/platform_macros.h>
#include <assert.h>     /* assert */
#include <thread>
#include <set>
#include <math.h>  
#include <numeric> // for std::accumulate
#include <cmath>   // for std::mean and std::stdev
#include <nlohmann/json.hpp>
#include "utils.cpp"

// create indices for debugging, write indices to file, and get recall stats for all queries
int main(int argc, char *argv[]) {
    unsigned int nthreads = std::thread::hardware_concurrency();
    std::cout << "====================\nSTART: running TEST_ACORN for hnsw, sift data --" << nthreads << "cores\n" << std::endl;
    // printf("====================\nSTART: running MAKE_INDICES for hnsw --...\n");
    double t0 = elapsed();
    
    std::string data_path;
    std::string save_path;
    int efc = 40; // default is 40
    int efs = 16; //  default is 16
    int k = 10; // search parameter
    size_t d = 128; // dimension of the vectors to index - will be overwritten by the dimension of the dataset
    int M; // HSNW param M TODO change M back
    int M_beta; // param for compression
    // float attr_sel = 0.001;
    // int gamma = (int) 1 / attr_sel;
    int gamma;
    int n_centroids;
    // int filter = 0;
    std::string dataset; // must be sift1B or sift1M or tripclick
    int test_partitions = 0;
    int step = 10; //2
    
    std::string assignment_type = "rand";
    int alpha = 0;

    srand(0); // seed for random number generator
    int num_trials = 60;


    size_t N = 0; // N will be how many we truncate nb from sift1M to

    int opt;
    {// parse arguments

        if (argc < 8 || argc > 10) {
            fprintf(stderr, "Syntax: %s <data_path> <save_path> <number vecs> <gamma> [<assignment_type>] [<alpha>] <dataset> <M> <M_beta>\n", argv[0]);
            exit(1);
        }

        data_path = argv[1];
        printf("data_path: %s\n", data_path.c_str());


        save_path = argv[2];
        printf("save_path: %s\n", save_path.c_str());


        N = strtoul(argv[3], NULL, 10);
        printf("N: %ld\n", N);
     

        gamma = atoi(argv[4]);
        printf("gamma: %d\n", gamma);

        
        dataset = argv[5];
        printf("dataset: %s\n", dataset.c_str());
        

        M = atoi(argv[6]);
        printf("M: %d\n", M);


        M_beta = atoi(argv[7]);
        printf("M_beta: %d\n", M_beta);

    }


    // load metadata
    n_centroids = gamma;

    // according to https://github.com/TAG-Research/ACORN/issues/2 
    // metadata does not influnce the ACORN algorithm 
    // (maybe needed for HNSW)
    std::vector<int> metadata(N,0);


    size_t nq;
    float* xq;
    { // load query vectors and attributes
        printf("[%.3f s] Loading query vectors and attributes\n", elapsed() - t0);

        size_t d2;
        bool is_base = 0;
        std::string filename = data_path + "query.fvecs";
        xq = fvecs_read(filename.c_str(), &d2, &nq);
        assert(d == d2 || !"query does not have same dimension as expected 128");
        if (d != d2) {
            d = d2;
        }
        
        std::cout << "query vecs data loaded, with dim: " << d2 << ", nb=" << nq << std::endl;
        printf("[%.3f s] Loaded query vectors from %s\n", elapsed() - t0, filename.c_str());
 
    }

    // create normal (base) and hybrid index
    printf("[%.3f s] Index Params -- d: %ld, M: %d, N: %ld, gamma: %d\n",
               elapsed() - t0, d, M, N, gamma);

    // ACORN-gamma
    faiss::IndexACORNFlat hybrid_index(d, M, gamma, metadata, M_beta);
    hybrid_index.acorn.efSearch = efs; // default is 16 HybridHNSW.capp
    debug("ACORN index created%s\n", "");


    { // populating the database
        std::cout << "====================Vectors====================\n" << std::endl;
        // printf("====================Vectors====================\n");
       
        printf("[%.3f s] Loading database\n", elapsed() - t0);

        size_t nb, d2;
        std::string filename =  data_path + "base.fvecs";
        float* xb = fvecs_read(filename.c_str(), &d2, &nb);
        assert(d == d2 || !"dataset does not dim 128 as expected");
        printf("[%.3f s] Loaded base vectors from file: %s\n", elapsed() - t0, filename.c_str());


        std::cout << "data loaded, with dim: " << d2 << ", nb=" << nb << std::endl;

        printf("[%.3f s] Indexing database, size %ld*%ld from max %ld\n",
               elapsed() - t0, N, d2, nb);


        hybrid_index.add(N, xb);
        printf("[%.3f s] Vectors added to hybrid index \n", elapsed() - t0);
        std::cout << "Hybrid index vectors added" << nb << std::endl;    

        delete[] xb;       
    }


       // write hybrid index and partition indices to files
    {
        std::cout << "====================Write Index====================\n" << std::endl;
        // write hybrid index
        // std::string filename = "hybrid_index" + dataset + ".index";
        std::stringstream filepath_stream;

        filepath_stream << save_path << "_M=" << M << "_efc" << efc << "_Mb=" << M_beta << "_gamma=" << gamma << ".json";

        std::string filepath = filepath_stream.str();
        write_index(&hybrid_index, filepath.c_str());
        printf("[%.3f s] Wrote hybrid index to file: %s\n", elapsed() - t0, filepath.c_str());      

        
    }


    { // print out stats
    
        printf("====================================\n");
        printf("============ ACORN INDEX =============\n");
        printf("====================================\n");
        hybrid_index.printStats(false);
       
    }
    
}