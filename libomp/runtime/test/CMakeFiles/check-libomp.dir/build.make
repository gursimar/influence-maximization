# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.10.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.10.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp

# Utility rule file for check-libomp.

# Include the progress variables for this target.
include runtime/test/CMakeFiles/check-libomp.dir/progress.make

runtime/test/CMakeFiles/check-libomp:
	cd /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/test && /usr/local/Cellar/cmake/3.10.1/bin/cmake -E echo check-libomp\ does\ nothing,\ dependencies\ not\ found.

check-libomp: runtime/test/CMakeFiles/check-libomp
check-libomp: runtime/test/CMakeFiles/check-libomp.dir/build.make

.PHONY : check-libomp

# Rule to build all files generated by this target.
runtime/test/CMakeFiles/check-libomp.dir/build: check-libomp

.PHONY : runtime/test/CMakeFiles/check-libomp.dir/build

runtime/test/CMakeFiles/check-libomp.dir/clean:
	cd /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/test && $(CMAKE_COMMAND) -P CMakeFiles/check-libomp.dir/cmake_clean.cmake
.PHONY : runtime/test/CMakeFiles/check-libomp.dir/clean

runtime/test/CMakeFiles/check-libomp.dir/depend:
	cd /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/test /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/test /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/test/CMakeFiles/check-libomp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : runtime/test/CMakeFiles/check-libomp.dir/depend

