cmake_minimum_required(VERSION 3.10)
project(WaterfallPiano VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/WaterfallPiano.cpp
    src/MidiParser.cpp
)

# Create executable
add_executable(waterfall-piano ${SOURCES})

# Link libraries
target_link_libraries(waterfall-piano ${SDL2_LIBRARIES})

# Installation
install(TARGETS waterfall-piano DESTINATION bin)

# Windows-specific settings
if(WIN32)
    # Copy SDL2 DLL to build directory on Windows
    if(EXISTS "${SDL2_PREFIX}/bin/SDL2.dll")
        add_custom_command(TARGET waterfall-piano POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_PREFIX}/bin/SDL2.dll"
            $<TARGET_FILE_DIR:waterfall-piano>)
    endif()
endif()

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "SDL2 Libraries: ${SDL2_LIBRARIES}")
