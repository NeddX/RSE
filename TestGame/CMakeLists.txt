# CMakeList.txt : CMake project for TestGame, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (TestGame 
	"src/main.cpp" 
	"src/include/EnemyAI.h"
	"src/include/PlayerController.h"
	"src/include/Projectile.h"
	"src/include/Wall.h"
)

target_link_libraries(TestGame RSE)

if (MSVC)
	target_compile_options(TestGame PRIVATE /INCREMENTAL)
	target_compile_options(TestGame PRIVATE /ZI)
endif()

#if (CMAKE_VERSION VERSION_GREATER 3.12)
  #set_property(TARGET RSE PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Add tests and install targets if needed.
