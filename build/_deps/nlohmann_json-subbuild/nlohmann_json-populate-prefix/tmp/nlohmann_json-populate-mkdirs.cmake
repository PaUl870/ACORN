# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/workspace/benchmark/code/ACORN/ACORN/build/_deps/nlohmann_json-src"
  "/workspace/benchmark/code/ACORN/ACORN/build/_deps/nlohmann_json-build"
  "/workspace/benchmark/code/ACORN/ACORN/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix"
  "/workspace/benchmark/code/ACORN/ACORN/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/tmp"
  "/workspace/benchmark/code/ACORN/ACORN/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src/nlohmann_json-populate-stamp"
  "/workspace/benchmark/code/ACORN/ACORN/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src"
  "/workspace/benchmark/code/ACORN/ACORN/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src/nlohmann_json-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/workspace/benchmark/code/ACORN/ACORN/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src/nlohmann_json-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/workspace/benchmark/code/ACORN/ACORN/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src/nlohmann_json-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
