# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspace/benchmark/code/ACORN/ACORN

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspace/benchmark/code/ACORN/ACORN/build

# Include any dependencies generated for this target.
include tutorial/cpp/CMakeFiles/2-IVFFlat.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tutorial/cpp/CMakeFiles/2-IVFFlat.dir/compiler_depend.make

# Include the progress variables for this target.
include tutorial/cpp/CMakeFiles/2-IVFFlat.dir/progress.make

# Include the compile flags for this target's objects.
include tutorial/cpp/CMakeFiles/2-IVFFlat.dir/flags.make

tutorial/cpp/CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.o: tutorial/cpp/CMakeFiles/2-IVFFlat.dir/flags.make
tutorial/cpp/CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.o: /workspace/benchmark/code/ACORN/ACORN/tutorial/cpp/2-IVFFlat.cpp
tutorial/cpp/CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.o: tutorial/cpp/CMakeFiles/2-IVFFlat.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/workspace/benchmark/code/ACORN/ACORN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tutorial/cpp/CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.o"
	cd /workspace/benchmark/code/ACORN/ACORN/build/tutorial/cpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tutorial/cpp/CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.o -MF CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.o.d -o CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.o -c /workspace/benchmark/code/ACORN/ACORN/tutorial/cpp/2-IVFFlat.cpp

tutorial/cpp/CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.i"
	cd /workspace/benchmark/code/ACORN/ACORN/build/tutorial/cpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/benchmark/code/ACORN/ACORN/tutorial/cpp/2-IVFFlat.cpp > CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.i

tutorial/cpp/CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.s"
	cd /workspace/benchmark/code/ACORN/ACORN/build/tutorial/cpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/benchmark/code/ACORN/ACORN/tutorial/cpp/2-IVFFlat.cpp -o CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.s

# Object files for target 2-IVFFlat
2__IVFFlat_OBJECTS = \
"CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.o"

# External object files for target 2-IVFFlat
2__IVFFlat_EXTERNAL_OBJECTS =

tutorial/cpp/2-IVFFlat: tutorial/cpp/CMakeFiles/2-IVFFlat.dir/2-IVFFlat.cpp.o
tutorial/cpp/2-IVFFlat: tutorial/cpp/CMakeFiles/2-IVFFlat.dir/build.make
tutorial/cpp/2-IVFFlat: faiss/libfaiss.so
tutorial/cpp/2-IVFFlat: /usr/lib/x86_64-linux-gnu/libz.so
tutorial/cpp/2-IVFFlat: tutorial/cpp/CMakeFiles/2-IVFFlat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/workspace/benchmark/code/ACORN/ACORN/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 2-IVFFlat"
	cd /workspace/benchmark/code/ACORN/ACORN/build/tutorial/cpp && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/2-IVFFlat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tutorial/cpp/CMakeFiles/2-IVFFlat.dir/build: tutorial/cpp/2-IVFFlat
.PHONY : tutorial/cpp/CMakeFiles/2-IVFFlat.dir/build

tutorial/cpp/CMakeFiles/2-IVFFlat.dir/clean:
	cd /workspace/benchmark/code/ACORN/ACORN/build/tutorial/cpp && $(CMAKE_COMMAND) -P CMakeFiles/2-IVFFlat.dir/cmake_clean.cmake
.PHONY : tutorial/cpp/CMakeFiles/2-IVFFlat.dir/clean

tutorial/cpp/CMakeFiles/2-IVFFlat.dir/depend:
	cd /workspace/benchmark/code/ACORN/ACORN/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace/benchmark/code/ACORN/ACORN /workspace/benchmark/code/ACORN/ACORN/tutorial/cpp /workspace/benchmark/code/ACORN/ACORN/build /workspace/benchmark/code/ACORN/ACORN/build/tutorial/cpp /workspace/benchmark/code/ACORN/ACORN/build/tutorial/cpp/CMakeFiles/2-IVFFlat.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tutorial/cpp/CMakeFiles/2-IVFFlat.dir/depend

