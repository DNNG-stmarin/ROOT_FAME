# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.16.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.16.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/isabelhernandez/Desktop/root

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/isabelhernandez/Desktop/root

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/3.16.3/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/3.16.3/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/isabelhernandez/Desktop/root/CMakeFiles /Users/isabelhernandez/Desktop/root/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/isabelhernandez/Desktop/root/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named ROOT_FAME

# Build rule for target.
ROOT_FAME: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ROOT_FAME
.PHONY : ROOT_FAME

# fast build rule for target.
ROOT_FAME/fast:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/build
.PHONY : ROOT_FAME/fast

main.o: main.cc.o

.PHONY : main.o

# target to build an object file
main.cc.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/main.cc.o
.PHONY : main.cc.o

main.i: main.cc.i

.PHONY : main.i

# target to preprocess a source file
main.cc.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/main.cc.i
.PHONY : main.cc.i

main.s: main.cc.s

.PHONY : main.s

# target to generate assembly for a file
main.cc.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/main.cc.s
.PHONY : main.cc.s

src/CoincidenceAnalysis.o: src/CoincidenceAnalysis.C.o

.PHONY : src/CoincidenceAnalysis.o

# target to build an object file
src/CoincidenceAnalysis.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.o
.PHONY : src/CoincidenceAnalysis.C.o

src/CoincidenceAnalysis.i: src/CoincidenceAnalysis.C.i

.PHONY : src/CoincidenceAnalysis.i

# target to preprocess a source file
src/CoincidenceAnalysis.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.i
.PHONY : src/CoincidenceAnalysis.C.i

src/CoincidenceAnalysis.s: src/CoincidenceAnalysis.C.s

.PHONY : src/CoincidenceAnalysis.s

# target to generate assembly for a file
src/CoincidenceAnalysis.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.s
.PHONY : src/CoincidenceAnalysis.C.s

src/DetectionAnalysis.o: src/DetectionAnalysis.C.o

.PHONY : src/DetectionAnalysis.o

# target to build an object file
src/DetectionAnalysis.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.o
.PHONY : src/DetectionAnalysis.C.o

src/DetectionAnalysis.i: src/DetectionAnalysis.C.i

.PHONY : src/DetectionAnalysis.i

# target to preprocess a source file
src/DetectionAnalysis.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.i
.PHONY : src/DetectionAnalysis.C.i

src/DetectionAnalysis.s: src/DetectionAnalysis.C.s

.PHONY : src/DetectionAnalysis.s

# target to generate assembly for a file
src/DetectionAnalysis.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.s
.PHONY : src/DetectionAnalysis.C.s

src/DetectorSystemClass.o: src/DetectorSystemClass.C.o

.PHONY : src/DetectorSystemClass.o

# target to build an object file
src/DetectorSystemClass.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.o
.PHONY : src/DetectorSystemClass.C.o

src/DetectorSystemClass.i: src/DetectorSystemClass.C.i

.PHONY : src/DetectorSystemClass.i

# target to preprocess a source file
src/DetectorSystemClass.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.i
.PHONY : src/DetectorSystemClass.C.i

src/DetectorSystemClass.s: src/DetectorSystemClass.C.s

.PHONY : src/DetectorSystemClass.s

# target to generate assembly for a file
src/DetectorSystemClass.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.s
.PHONY : src/DetectorSystemClass.C.s

src/FissionAnalysis.o: src/FissionAnalysis.C.o

.PHONY : src/FissionAnalysis.o

# target to build an object file
src/FissionAnalysis.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.o
.PHONY : src/FissionAnalysis.C.o

src/FissionAnalysis.i: src/FissionAnalysis.C.i

.PHONY : src/FissionAnalysis.i

# target to preprocess a source file
src/FissionAnalysis.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.i
.PHONY : src/FissionAnalysis.C.i

src/FissionAnalysis.s: src/FissionAnalysis.C.s

.PHONY : src/FissionAnalysis.s

# target to generate assembly for a file
src/FissionAnalysis.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.s
.PHONY : src/FissionAnalysis.C.s

src/FissionExperimentClass.o: src/FissionExperimentClass.C.o

.PHONY : src/FissionExperimentClass.o

# target to build an object file
src/FissionExperimentClass.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.o
.PHONY : src/FissionExperimentClass.C.o

src/FissionExperimentClass.i: src/FissionExperimentClass.C.i

.PHONY : src/FissionExperimentClass.i

# target to preprocess a source file
src/FissionExperimentClass.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.i
.PHONY : src/FissionExperimentClass.C.i

src/FissionExperimentClass.s: src/FissionExperimentClass.C.s

.PHONY : src/FissionExperimentClass.s

# target to generate assembly for a file
src/FissionExperimentClass.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.s
.PHONY : src/FissionExperimentClass.C.s

src/InitializeDetectorHistograms.o: src/InitializeDetectorHistograms.C.o

.PHONY : src/InitializeDetectorHistograms.o

# target to build an object file
src/InitializeDetectorHistograms.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.o
.PHONY : src/InitializeDetectorHistograms.C.o

src/InitializeDetectorHistograms.i: src/InitializeDetectorHistograms.C.i

.PHONY : src/InitializeDetectorHistograms.i

# target to preprocess a source file
src/InitializeDetectorHistograms.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.i
.PHONY : src/InitializeDetectorHistograms.C.i

src/InitializeDetectorHistograms.s: src/InitializeDetectorHistograms.C.s

.PHONY : src/InitializeDetectorHistograms.s

# target to generate assembly for a file
src/InitializeDetectorHistograms.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.s
.PHONY : src/InitializeDetectorHistograms.C.s

src/InitializePSDFunctions.o: src/InitializePSDFunctions.C.o

.PHONY : src/InitializePSDFunctions.o

# target to build an object file
src/InitializePSDFunctions.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.o
.PHONY : src/InitializePSDFunctions.C.o

src/InitializePSDFunctions.i: src/InitializePSDFunctions.C.i

.PHONY : src/InitializePSDFunctions.i

# target to preprocess a source file
src/InitializePSDFunctions.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.i
.PHONY : src/InitializePSDFunctions.C.i

src/InitializePSDFunctions.s: src/InitializePSDFunctions.C.s

.PHONY : src/InitializePSDFunctions.s

# target to generate assembly for a file
src/InitializePSDFunctions.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.s
.PHONY : src/InitializePSDFunctions.C.s

src/MappingFunctions.o: src/MappingFunctions.C.o

.PHONY : src/MappingFunctions.o

# target to build an object file
src/MappingFunctions.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.o
.PHONY : src/MappingFunctions.C.o

src/MappingFunctions.i: src/MappingFunctions.C.i

.PHONY : src/MappingFunctions.i

# target to preprocess a source file
src/MappingFunctions.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.i
.PHONY : src/MappingFunctions.C.i

src/MappingFunctions.s: src/MappingFunctions.C.s

.PHONY : src/MappingFunctions.s

# target to generate assembly for a file
src/MappingFunctions.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.s
.PHONY : src/MappingFunctions.C.s

src/SystemAnalysis.o: src/SystemAnalysis.C.o

.PHONY : src/SystemAnalysis.o

# target to build an object file
src/SystemAnalysis.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.o
.PHONY : src/SystemAnalysis.C.o

src/SystemAnalysis.i: src/SystemAnalysis.C.i

.PHONY : src/SystemAnalysis.i

# target to preprocess a source file
src/SystemAnalysis.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.i
.PHONY : src/SystemAnalysis.C.i

src/SystemAnalysis.s: src/SystemAnalysis.C.s

.PHONY : src/SystemAnalysis.s

# target to generate assembly for a file
src/SystemAnalysis.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.s
.PHONY : src/SystemAnalysis.C.s

src/TriggerAnalysis.o: src/TriggerAnalysis.C.o

.PHONY : src/TriggerAnalysis.o

# target to build an object file
src/TriggerAnalysis.C.o:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.o
.PHONY : src/TriggerAnalysis.C.o

src/TriggerAnalysis.i: src/TriggerAnalysis.C.i

.PHONY : src/TriggerAnalysis.i

# target to preprocess a source file
src/TriggerAnalysis.C.i:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.i
.PHONY : src/TriggerAnalysis.C.i

src/TriggerAnalysis.s: src/TriggerAnalysis.C.s

.PHONY : src/TriggerAnalysis.s

# target to generate assembly for a file
src/TriggerAnalysis.C.s:
	$(MAKE) -f CMakeFiles/ROOT_FAME.dir/build.make CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.s
.PHONY : src/TriggerAnalysis.C.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... ROOT_FAME"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... src/CoincidenceAnalysis.o"
	@echo "... src/CoincidenceAnalysis.i"
	@echo "... src/CoincidenceAnalysis.s"
	@echo "... src/DetectionAnalysis.o"
	@echo "... src/DetectionAnalysis.i"
	@echo "... src/DetectionAnalysis.s"
	@echo "... src/DetectorSystemClass.o"
	@echo "... src/DetectorSystemClass.i"
	@echo "... src/DetectorSystemClass.s"
	@echo "... src/FissionAnalysis.o"
	@echo "... src/FissionAnalysis.i"
	@echo "... src/FissionAnalysis.s"
	@echo "... src/FissionExperimentClass.o"
	@echo "... src/FissionExperimentClass.i"
	@echo "... src/FissionExperimentClass.s"
	@echo "... src/InitializeDetectorHistograms.o"
	@echo "... src/InitializeDetectorHistograms.i"
	@echo "... src/InitializeDetectorHistograms.s"
	@echo "... src/InitializePSDFunctions.o"
	@echo "... src/InitializePSDFunctions.i"
	@echo "... src/InitializePSDFunctions.s"
	@echo "... src/MappingFunctions.o"
	@echo "... src/MappingFunctions.i"
	@echo "... src/MappingFunctions.s"
	@echo "... src/SystemAnalysis.o"
	@echo "... src/SystemAnalysis.i"
	@echo "... src/SystemAnalysis.s"
	@echo "... src/TriggerAnalysis.o"
	@echo "... src/TriggerAnalysis.i"
	@echo "... src/TriggerAnalysis.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

