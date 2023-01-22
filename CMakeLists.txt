# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("RSE")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")

# Include sub-projects.
add_subdirectory ("RSE")
add_subdirectory ("TestGame")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT <project_name>)