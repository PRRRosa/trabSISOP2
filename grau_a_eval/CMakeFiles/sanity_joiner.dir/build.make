# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel

# Include any dependencies generated for this target.
include grau_a_eval/CMakeFiles/sanity_joiner.dir/depend.make

# Include the progress variables for this target.
include grau_a_eval/CMakeFiles/sanity_joiner.dir/progress.make

# Include the compile flags for this target's objects.
include grau_a_eval/CMakeFiles/sanity_joiner.dir/flags.make

grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o: grau_a_eval/CMakeFiles/sanity_joiner.dir/flags.make
grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o: grau_a_eval/sanity_joiner.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o"
	cd /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o   -c /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval/sanity_joiner.c

grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sanity_joiner.dir/sanity_joiner.c.i"
	cd /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval/sanity_joiner.c > CMakeFiles/sanity_joiner.dir/sanity_joiner.c.i

grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sanity_joiner.dir/sanity_joiner.c.s"
	cd /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval/sanity_joiner.c -o CMakeFiles/sanity_joiner.dir/sanity_joiner.c.s

grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o.requires:
.PHONY : grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o.requires

grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o.provides: grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o.requires
	$(MAKE) -f grau_a_eval/CMakeFiles/sanity_joiner.dir/build.make grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o.provides.build
.PHONY : grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o.provides

grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o.provides.build: grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o

# Object files for target sanity_joiner
sanity_joiner_OBJECTS = \
"CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o"

# External object files for target sanity_joiner
sanity_joiner_EXTERNAL_OBJECTS =

grau_a_eval/sanity_joiner: grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o
grau_a_eval/sanity_joiner: grau_a_eval/CMakeFiles/sanity_joiner.dir/build.make
grau_a_eval/sanity_joiner: libthread/libthread.so
grau_a_eval/sanity_joiner: grau_a_eval/CMakeFiles/sanity_joiner.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable sanity_joiner"
	cd /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sanity_joiner.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grau_a_eval/CMakeFiles/sanity_joiner.dir/build: grau_a_eval/sanity_joiner
.PHONY : grau_a_eval/CMakeFiles/sanity_joiner.dir/build

grau_a_eval/CMakeFiles/sanity_joiner.dir/requires: grau_a_eval/CMakeFiles/sanity_joiner.dir/sanity_joiner.c.o.requires
.PHONY : grau_a_eval/CMakeFiles/sanity_joiner.dir/requires

grau_a_eval/CMakeFiles/sanity_joiner.dir/clean:
	cd /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval && $(CMAKE_COMMAND) -P CMakeFiles/sanity_joiner.dir/cmake_clean.cmake
.PHONY : grau_a_eval/CMakeFiles/sanity_joiner.dir/clean

grau_a_eval/CMakeFiles/sanity_joiner.dir/depend:
	cd /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval /home/osboxes/Desktop/trabGrauA-SisopII/graua-skel/grau_a_eval/CMakeFiles/sanity_joiner.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grau_a_eval/CMakeFiles/sanity_joiner.dir/depend

