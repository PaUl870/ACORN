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
    int k;
    int efc = 40; // default is 40
    int efs =   32; //  default is 16
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

        if (argc < 9 || argc > 11) {
            fprintf(stderr, "Syntax: %s <data_path> <save_path> <number vecs> <k> <gamma> [<assignment_type>] [<alpha>] <dataset> <M> <M_beta>\n", argv[0]);
            exit(1);
        }

        data_path = argv[1];
        printf("data_path: %s\n", data_path.c_str());


        save_path = argv[2];
        printf("save_path: %s\n", save_path.c_str());


        N = strtoul(argv[3], NULL, 10);
        printf("N: %ld\n", N);
     

        k = atoi(argv[4]);
        printf("k: %d\n", k);


        gamma = atoi(argv[5]);
        printf("gamma: %d\n", gamma);

        
        dataset = argv[6];
        printf("dataset: %s\n", dataset.c_str());
        

        M = atoi(argv[7]);
        printf("M: %d\n", M);


        M_beta = atoi(argv[8]);
        printf("M_beta: %d\n", M_beta);

    }


    n_centroids = gamma;
    std::vector<std::vector<std::string>> metadata;
    std::vector<std::vector<std::string>> aq;
    std::vector<int> gt;

    { // load attributes
        printf("[%.3f s] Loading attributes\n", elapsed() - t0);


        std::string filename = data_path + "label_base.txt";
        read_txt(filename.c_str(), metadata);
        printf("[%.3f s] Loaded metadata, %ld attr's found\n", 
        elapsed() - t0, metadata[0].size());


        filename = data_path + "label_query.txt";
        read_txt(filename.c_str(), aq);
        printf("[%.3f s] Loaded query attributes, %ld attr's found\n", 
        elapsed() - t0, aq[0].size());
        

        filename = data_path + "label_groundtruth.ivecs";
        int num, dim;
        read_ivecs(filename.c_str(), gt, num, dim, k);
        printf("[%.3f s] Loaded ground truth, gt_size: %d\n", elapsed() - t0, k);
    }



    size_t nq;
    float* xq;
    { // load query vectors
        printf("[%.3f s] Loading query vectors\n", elapsed() - t0);

        size_t d2;
        std::string filename = data_path + "query.fvecs";
        xq = fvecs_read(filename.c_str(), &d2, &nq);
        assert(d == d2 || !"query does not have same dimension as expected 128");
        if (d != d2) {
            d = d2;
        }
        
        std::cout << "query vecs data loaded, with dim: " << d2 << ", nb=" << nq << std::endl;
        printf("[%.3f s] Loaded query vectors from %s\n", elapsed() - t0, filename.c_str());
 
    }

    printf("[%.3f s] Loading index\n", elapsed() - t0);
    std::stringstream filepath_stream;
    filepath_stream << save_path << "_M=" << M << "_efc" << efc << "_Mb=" << M_beta << "_gamma=" << gamma << ".json";
    std::string filepath = filepath_stream.str();
    auto& hybrid_index = *dynamic_cast<faiss::IndexACORNFlat*>(faiss::read_index(filepath.c_str()));
    printf("[%.3f s] Loaded index from %s\n", elapsed() - t0, filepath.c_str());


    { // searching the hybrid database
        hybrid_index.acorn.efSearch = efs;
        printf("==================== ACORN INDEX ====================\n");
        printf("[%.3f s] Searching the %d nearest neighbors "
               "of %ld vectors in the index, efsearch %d\n",
               elapsed() - t0,
               k,
               nq,
               hybrid_index.acorn.efSearch);

        std::vector<faiss::idx_t> nns2(k * nq);
        std::vector<float> dis2(k * nq);

        // create filter_ids_map, ie a bitmap of the ids that are in the filter
        double t1_f = elapsed();
        std::vector<char> filter_ids_map(nq * N);
        for (int xq = 0; xq < nq; xq++) {
            for (int xb = 0; xb < N; xb++) {
                filter_ids_map[xq * N + xb] = (bool) (metadata[xb] == aq[xq]);
            }
        }
        double t2_f = elapsed();

        double t1_x = elapsed();
        hybrid_index.search(nq, xq, k, dis2.data(), nns2.data(), filter_ids_map.data()); // TODO change first argument back to nq
        double t2_x = elapsed();



        printf("[%.3f s] *** Query time: %f\n",
               elapsed() - t0, t2_x - t1_x);

        printf("[%.3f s] *** Query time (adjusted for prefilter): %f\n",
               elapsed() - t0, t2_x - t1_x + t2_f - t1_f);

        uint32_t count = 0;
        int* answer = gt.data();
        faiss::idx_t* guess = nns2.data();

        for (int i=0;i<nq;i++){
            std::sort(answer+ k*i, answer + (i+1)*k);
            std::sort(guess+ k*i, guess+ (i+1)*k);
            std::vector<int> tmp;
            std::set_intersection(answer+ k*i, answer + (i+1)*k,  // Input iterators for first range 
                                guess+ k*i, guess+ (i+1)*k, // Input iterators for second range 
                                std::back_inserter(tmp));
            count += double(tmp.size());
        }

        double recall = (count/double(nq*k));

        printf("[%.3f s] *** Recall %f\n",
               elapsed() - t0, recall);
    }


    {// compute recall


    }

    // look at stats
    const faiss::ACORNStats& stats = faiss::acorn_stats;

    {
        std::cout << "============= ACORN QUERY PROFILING STATS =============" << std::endl;
        printf("[%.3f s] Timing results for search of k=%d nearest neighbors of nq=%ld vectors in the index\n",
                elapsed() - t0,
                k,
                nq);
        std::cout << "n1: " << stats.n1 << std::endl;
        std::cout << "n2: " << stats.n2 << std::endl;
        std::cout << "n3 (number distance comps at level 0): " << stats.n3 << std::endl;
        std::cout << "ndis: " << stats.ndis << std::endl;
        std::cout << "nreorder: " << stats.nreorder << std::endl;
        printf("average distance computations per query: %f\n", (float)stats.n3 / stats.n1);
    }
  
    
    printf("[%.3f s] -----DONE-----\n", elapsed() - t0);




}