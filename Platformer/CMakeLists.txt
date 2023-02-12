# CMakeList.txt : CMake project for Platformer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Platformer 
	"src/main.cpp" 
	"src/include/PlayerController.h"
)

target_link_libraries(Platformer RSE)

if (MSVC)
	target_compile_options(Platformer PRIVATE /INCREMENTAL)
	target_compile_options(Platformer PRIVATE /ZI)
endif()

#if (CMAKE_VERSION VERSION_GREATER 3.12)
  #set_property(TARGET RSE PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Add tests and install targets if needed.
