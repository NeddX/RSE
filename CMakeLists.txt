# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("RSE")

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox") # TODO: remove!
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include sub-projects.
add_subdirectory ("RSE")
add_subdirectory ("TestGame")
add_subdirectory ("Platformer")
#add_subdirectory ("RCLRW")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT TestGame)
