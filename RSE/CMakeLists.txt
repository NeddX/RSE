set(CMAKE_CXX_STANDARD 20)

add_library (RSE STATIC
	
	"src/Core/include/Core.h" 
	"src/Core/Core.cpp" 
	"src/Core/include/Vector2.h" 
	"src/Core/include/ResourceHandler.h" 
	"src/Core/ResourceHandler.cpp" 
	"src/Core/include/RException.h" 
	"src/Core/RException.cpp" 
	"src/Core/include/Map.h" 
	"src/Core/Map.cpp"
	"src/Core/ECS/include/ECS.h"
	"src/Core/ECS/ECS.cpp"
	"src/Core/ECS/include/TransformComponent.h"
	"src/Core/ECS/TransformComponent.cpp"
	"src/Core/ECS/include/Sprite.h"
	"src/Core/ECS/Sprite.cpp"
	"src/Core/ECS/include/BoxCollider2D.h"
	"src/Core/ECS/BoxCollider2D.cpp"
	"src/Core/ECS/include/Components.h"
	"src/Core/ECS/include/Behaviour.h"
	"src/Core/include/Input.h"
	"src/Core/Input.cpp"
	"src/Core/include/Transform.h"
	"src/Core/include/Texture2D.h"
	"src/Core/Texture2D.cpp"
	"src/Core/include/Collision.h"
	"src/Core/ECS/include/TileMapComponent.h"
	"src/Core/include/Serializer.h"
	"src/Core/Serializer.cpp"
	"src/Core/ECS/include/SpriteSheetAnimation.h"
	"src/Core/ECS/SpriteSheetAnimation.cpp"
	"src/Core/ECS/include/Camera2DComponent.h"
	"src/Core/ECS/Camera2DComponent.cpp"
	"src/Core/include/Geometry.h"
	"src/Math/include/Mathf.h"
	"src/Util/xstdf.h"
	"include/RSE.h"
)

# Define so that GLEW will know that we're using STATIC version of it's library
target_compile_definitions(RSE PRIVATE GLEW_STATIC)

# Link OpenGL Windows/Unix
find_package(OpenGL REQUIRED)
if (UNIX)
	find_package(GLUT REQUIRED)
endif()
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(RSE ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# Add yaml-cpp project
add_subdirectory(${PROJECT_SOURCE_DIR}/RSE/external/lib/yaml-cpp-master)

# Add and link sdafx.h (stl pch)
set(PCH_HEADER ${PROJECT_SOURCE_DIR}/RSE/pch/sdafx.h)
target_precompile_headers(RSE PUBLIC ${PCH_HEADER})

# Link SDL stuff
target_link_libraries(RSE ${PROJECT_SOURCE_DIR}/RSE/external/lib/SDL2/lib/x64/SDL2.lib)
target_link_libraries(RSE ${PROJECT_SOURCE_DIR}/RSE/external/lib/SDL2/lib/x64/SDL2main.lib)
target_link_libraries(RSE ${PROJECT_SOURCE_DIR}/RSE/external/lib/SDL2_image/lib/x64/SDL2_image.lib)
target_link_libraries(RSE ${PROJECT_SOURCE_DIR}/RSE/external/lib/glew/lib/Release/x64/glew32s.lib)
target_link_libraries(RSE yaml-cpp)
include_directories(${PROJECT_SOURCE_DIR}/RSE/external/lib/SDL2/include)
include_directories(${PROJECT_SOURCE_DIR}/RSE/external/lib/SDL2_image/include)
include_directories(${PROJECT_SOURCE_DIR}/RSE/external/lib/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/RSE/external/lib/boost)
include_directories(${PROJECT_SOURCE_DIR}/RSE/external/lib/yaml-cpp-master/include)
include_directories(${PROJECT_SOURCE_DIR}/RSE/external/include)
include_directories(${PROJECT_SOURCE_DIR}/RSE/pch/)

include_directories(${PROJECT_SOURCE_DIR}/RSE/src/Core/include)
include_directories(${PROJECT_SOURCE_DIR}/RSE/src/Math/include)

target_include_directories(RSE PUBLIC include/)
target_include_directories(RSE PUBLIC ${PROJECT_SOURCE_DIR}/RSE/external/lib/SDL2/include)
target_include_directories(RSE PUBLIC ${PROJECT_SOURCE_DIR}/RSE/external/lib/SDL2_image/include)
target_include_directories(RSE PUBLIC ${PROJECT_SOURCE_DIR}/RSE/external/lib/boost)
target_include_directories(RSE PUBLIC ${PROJECT_SOURCE_DIR}/RSE/external/lib/yaml-cpp-master/include)
target_include_directories(RSE PUBLIC ${PROJECT_SOURCE_DIR}/RSE/external/lib/glew/include)
target_include_directories(RSE PUBLIC ${PROJECT_SOURCE_DIR}/RSE/pch/)