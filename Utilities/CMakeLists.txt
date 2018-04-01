cmake_minimum_required(VERSION 3.10)
project(Utilities VERSION 0.1.0 LANGUAGES C CXX)

# Add library to the project using the specified source files:
add_library(Utilities
	include/Utilities/EngineException.h
    source/EngineException.cpp
)
add_library(Maia::Utilities ALIAS Utilities)

# Define headers:
target_include_directories(Utilities PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:Include>
    PRIVATE Source
)

# Specify rules to run at install time:
install(TARGETS Utilities 
	EXPORT UtilitiesConfig
	RUNTIME DESTINATION Binaries
	LIBRARY DESTINATION Libraries
	ARCHIVE DESTINATION Libraries/Static
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX})

# Make the library importable from the install directory:
install(EXPORT UtilitiesConfig DESTINATION Shared/Utilities/CMake)

# Make the project importable from the build directory:
export(TARGETS Utilities FILE UtilitiesConfig.cmake)
