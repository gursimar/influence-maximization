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

# Utility rule file for libomp-needed-headers.

# Include the progress variables for this target.
include runtime/src/CMakeFiles/libomp-needed-headers.dir/progress.make

runtime/src/CMakeFiles/libomp-needed-headers: runtime/src/kmp_i18n_id.inc
runtime/src/CMakeFiles/libomp-needed-headers: runtime/src/kmp_i18n_default.inc


runtime/src/kmp_i18n_id.inc: runtime/src/i18n/en_US.txt
runtime/src/kmp_i18n_id.inc: runtime/tools/message-converter.pl
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating kmp_i18n_id.inc"
	cd /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/src && /usr/bin/perl /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/tools/message-converter.pl --os=mac --prefix=kmp_i18n --enum=kmp_i18n_id.inc /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/src/i18n/en_US.txt

runtime/src/kmp_i18n_default.inc: runtime/src/i18n/en_US.txt
runtime/src/kmp_i18n_default.inc: runtime/tools/message-converter.pl
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating kmp_i18n_default.inc"
	cd /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/src && /usr/bin/perl /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/tools/message-converter.pl --os=mac --prefix=kmp_i18n --default=kmp_i18n_default.inc /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/src/i18n/en_US.txt

libomp-needed-headers: runtime/src/CMakeFiles/libomp-needed-headers
libomp-needed-headers: runtime/src/kmp_i18n_id.inc
libomp-needed-headers: runtime/src/kmp_i18n_default.inc
libomp-needed-headers: runtime/src/CMakeFiles/libomp-needed-headers.dir/build.make

.PHONY : libomp-needed-headers

# Rule to build all files generated by this target.
runtime/src/CMakeFiles/libomp-needed-headers.dir/build: libomp-needed-headers

.PHONY : runtime/src/CMakeFiles/libomp-needed-headers.dir/build

runtime/src/CMakeFiles/libomp-needed-headers.dir/clean:
	cd /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/src && $(CMAKE_COMMAND) -P CMakeFiles/libomp-needed-headers.dir/cmake_clean.cmake
.PHONY : runtime/src/CMakeFiles/libomp-needed-headers.dir/clean

runtime/src/CMakeFiles/libomp-needed-headers.dir/depend:
	cd /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/src /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/src /Users/simar/repos/CS534L-Social-Networks-Project/TIM/libomp/runtime/src/CMakeFiles/libomp-needed-headers.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : runtime/src/CMakeFiles/libomp-needed-headers.dir/depend

