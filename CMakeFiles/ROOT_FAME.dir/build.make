# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.16.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.16.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/isabelhernandez/Desktop/root

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/isabelhernandez/Desktop/root

# Include any dependencies generated for this target.
include CMakeFiles/ROOT_FAME.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ROOT_FAME.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ROOT_FAME.dir/flags.make

CMakeFiles/ROOT_FAME.dir/main.cc.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/main.cc.o: main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ROOT_FAME.dir/main.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/main.cc.o -c /Users/isabelhernandez/Desktop/root/main.cc

CMakeFiles/ROOT_FAME.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/main.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/main.cc > CMakeFiles/ROOT_FAME.dir/main.cc.i

CMakeFiles/ROOT_FAME.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/main.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/main.cc -o CMakeFiles/ROOT_FAME.dir/main.cc.s

CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.o: src/CoincidenceAnalysis.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.o -c /Users/isabelhernandez/Desktop/root/src/CoincidenceAnalysis.C

CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/CoincidenceAnalysis.C > CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.i

CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/CoincidenceAnalysis.C -o CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.s

CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.o: src/DetectionAnalysis.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.o -c /Users/isabelhernandez/Desktop/root/src/DetectionAnalysis.C

CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/DetectionAnalysis.C > CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.i

CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/DetectionAnalysis.C -o CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.s

CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.o: src/DetectorSystemClass.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.o -c /Users/isabelhernandez/Desktop/root/src/DetectorSystemClass.C

CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/DetectorSystemClass.C > CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.i

CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/DetectorSystemClass.C -o CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.s

CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.o: src/FissionAnalysis.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.o -c /Users/isabelhernandez/Desktop/root/src/FissionAnalysis.C

CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/FissionAnalysis.C > CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.i

CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/FissionAnalysis.C -o CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.s

CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.o: src/FissionAnalysisLoopNew.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.o -c /Users/isabelhernandez/Desktop/root/src/FissionAnalysisLoopNew.C

CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/FissionAnalysisLoopNew.C > CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.i

CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/FissionAnalysisLoopNew.C -o CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.s

CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.o: src/FissionExperimentClass.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.o -c /Users/isabelhernandez/Desktop/root/src/FissionExperimentClass.C

CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/FissionExperimentClass.C > CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.i

CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/FissionExperimentClass.C -o CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.s

CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.o: src/InitializeDetectorHistograms.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.o -c /Users/isabelhernandez/Desktop/root/src/InitializeDetectorHistograms.C

CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/InitializeDetectorHistograms.C > CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.i

CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/InitializeDetectorHistograms.C -o CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.s

CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.o: src/InitializePSDFunctions.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.o -c /Users/isabelhernandez/Desktop/root/src/InitializePSDFunctions.C

CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/InitializePSDFunctions.C > CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.i

CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/InitializePSDFunctions.C -o CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.s

CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.o: src/MappingFunctions.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.o -c /Users/isabelhernandez/Desktop/root/src/MappingFunctions.C

CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/MappingFunctions.C > CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.i

CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/MappingFunctions.C -o CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.s

CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.o: src/SystemAnalysis.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.o -c /Users/isabelhernandez/Desktop/root/src/SystemAnalysis.C

CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/SystemAnalysis.C > CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.i

CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/SystemAnalysis.C -o CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.s

CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.o: src/TriggerAnalysis.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.o -c /Users/isabelhernandez/Desktop/root/src/TriggerAnalysis.C

CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/TriggerAnalysis.C > CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.i

CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/TriggerAnalysis.C -o CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.s

CMakeFiles/ROOT_FAME.dir/src/temp.C.o: CMakeFiles/ROOT_FAME.dir/flags.make
CMakeFiles/ROOT_FAME.dir/src/temp.C.o: src/temp.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/ROOT_FAME.dir/src/temp.C.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ROOT_FAME.dir/src/temp.C.o -c /Users/isabelhernandez/Desktop/root/src/temp.C

CMakeFiles/ROOT_FAME.dir/src/temp.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ROOT_FAME.dir/src/temp.C.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/isabelhernandez/Desktop/root/src/temp.C > CMakeFiles/ROOT_FAME.dir/src/temp.C.i

CMakeFiles/ROOT_FAME.dir/src/temp.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ROOT_FAME.dir/src/temp.C.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/isabelhernandez/Desktop/root/src/temp.C -o CMakeFiles/ROOT_FAME.dir/src/temp.C.s

# Object files for target ROOT_FAME
ROOT_FAME_OBJECTS = \
"CMakeFiles/ROOT_FAME.dir/main.cc.o" \
"CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.o" \
"CMakeFiles/ROOT_FAME.dir/src/temp.C.o"

# External object files for target ROOT_FAME
ROOT_FAME_EXTERNAL_OBJECTS =

ROOT_FAME: CMakeFiles/ROOT_FAME.dir/main.cc.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/CoincidenceAnalysis.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/DetectionAnalysis.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/DetectorSystemClass.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/FissionAnalysis.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/FissionAnalysisLoopNew.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/FissionExperimentClass.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/InitializeDetectorHistograms.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/InitializePSDFunctions.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/MappingFunctions.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/SystemAnalysis.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/TriggerAnalysis.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/src/temp.C.o
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/build.make
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libCore.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libImt.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libRIO.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libNet.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libHist.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libGraf.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libGraf3d.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libGpad.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libROOTDataFrame.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libTree.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libTreePlayer.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libRint.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libPostscript.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libMatrix.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libPhysics.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libMathCore.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libThread.so
ROOT_FAME: /Users/isabelhernandez/ROOT/root/lib/libMultiProc.so
ROOT_FAME: CMakeFiles/ROOT_FAME.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/isabelhernandez/Desktop/root/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable ROOT_FAME"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ROOT_FAME.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ROOT_FAME.dir/build: ROOT_FAME

.PHONY : CMakeFiles/ROOT_FAME.dir/build

CMakeFiles/ROOT_FAME.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ROOT_FAME.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ROOT_FAME.dir/clean

CMakeFiles/ROOT_FAME.dir/depend:
	cd /Users/isabelhernandez/Desktop/root && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/isabelhernandez/Desktop/root /Users/isabelhernandez/Desktop/root /Users/isabelhernandez/Desktop/root /Users/isabelhernandez/Desktop/root /Users/isabelhernandez/Desktop/root/CMakeFiles/ROOT_FAME.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ROOT_FAME.dir/depend
