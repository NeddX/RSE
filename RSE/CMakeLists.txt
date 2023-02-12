add_library (RSE STATIC
	
	"src/Core/include/Core.h" 
	"src/Core/Core.cpp" 
	"src/Core/include/Vector2.h" 
	"src/Core/include/ResourceHandler.h" 
	"src/Core/ResourceHandler.cpp" 
	"src/Core/include/RException.h" 
	"src/Core/ECS/include/ECS.h"
	"src/Core/ECS/ECS.cpp"
	"src/Core/ECS/include/TransformComponent.h"
	"src/Core/ECS/TransformComponent.cpp"
	"src/Core/ECS/include/Sprite.h"
	"src/Core/ECS/Sprite.cpp"
	"src/Core/ECS/include/BoxCollider2D.h"
	"src/Core/ECS/BoxCollider2D.cpp"
	"src/Core/ECS/include/Components.h"
	"src/Core/ECS/include/NativeBehaviour.h"
	"src/Core/ECS/include/NativeBehaviourComponent.h"
	"src/Core/include/Input.h"
	"src/Core/Input.cpp"
	"src/Core/include/Transform.h"
	"src/Core/include/Texture2D.h"
	"src/Core/Texture2D.cpp"
	"src/Core/include/Collision.h"
	"src/Core/ECS/include/TilemapComponent.h"
	"src/Core/include/Serializer.h"
	"src/Core/Serializer.cpp"
	"src/Core/ECS/include/SpriteSheetAnimation.h"
	"src/Core/ECS/SpriteSheetAnimation.cpp"
	"src/Core/ECS/include/Camera2DComponent.h"
	"src/Core/ECS/Camera2DComponent.cpp"
	"src/Core/ECS/include/TilemapCollider2D.h"
	"src/Core/include/Geometry.h"
	"src/Math/include/Mathf.h"
	"src/Util/xstdf.h"
	"include/RSE.h"
)

# Set opengl preference
set(OpenGL_GL_PREFERENCE "GLVND")

# Setup few macros
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(RSE PUBLIC RSE_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions(RSE PRIVATE RSE_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Shipping")
	target_compile_definitions(RSE PRIVATE RSE_SHIPPING)
endif()

set(EXTERNAL_LIBRARIES ${PROJECT_SOURCE_DIR}/external/lib/)

# SDL
add_subdirectory(${EXTERNAL_LIBRARIES}/SDL ${PROJECT_BINARY_DIR}/SDL)
include_directories(${EXTERNAL_LIBRARIES}/SDL/include)
set(SDL_INCLUDE_DIRS ${EXTERNAL_LIBRARIES}/SDL/include)
set(SDL_LIBRARIES ${PROJECT_BINARY_DIR}/SDL)
target_link_libraries(RSE SDL2)
target_link_libraries(RSE SDL2main)

# SDL_image
add_subdirectory(${EXTERNAL_LIBRARIES}/SDL_image ${PROJECT_BINARY_DIR}/SDL_image)
include_directories(${EXTERNAL_LIBRARIES}/SDL_image)
set(SDL_INCLUDE_DIRS ${EXTERNAL_LIBRARIES}/SDL_image)
set(SDL_LIBRARIES ${PROJECT_BINARY_DIR}/SDL_image)
target_link_libraries(RSE SDL2_image)

# glew
#add_subdirectory(${PROJECT_SOURCE_DIR}/external/lib/glew/build/cmake ${PROJECT_BINARY_DIR}/glew)
#include_directories(${PROJECT_SOURCE_DIR}/external/lib/glew/include)
#set(GLEW_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/lib/glew/include)
#set(GLEW_LIBRARIES ${PROJECT_BINARY_DIR}/glew)
#target_link_libraries(RSE ${GLEW_LIBRARIES})

# fmt
add_subdirectory(${EXTERNAL_LIBRARIES}/fmt ${PROJECT_BINARY_DIR}/fmt)
include_directories(${EXTERNAL_LIBRARIES}/fmt/include)
set(FMT_INCLUDE_DIRS ${EXTERNAL_LIBRARIES}/fmt/include)
set(FMT_LIBRARIES ${PROJECT_BINARY_DIR}/fmt)
target_link_libraries(RSE fmt)

# yaml-cpp
add_subdirectory(${EXTERNAL_LIBRARIES}/yaml-cpp ${PROJECT_BINARY_DIR}/yaml-cpp)
include_directories(${EXTERNAL_LIBRARIES}/yaml-cpp/include)
set(YAML_CPP_INCLUDE_DIRS ${EXTERNAL_LIBRARIES}/yaml-cpp/include)
set(YAML_CPP_LIBRARIES ${PROJECT_BINARY_DIR}/yaml-cpp)
target_link_libraries(RSE yaml-cpp)

# boost filesystem
find_package(Boost REQUIRED COMPONENTS filesystem)
target_link_libraries(RSE Boost::filesystem)

# Define so that GLEW will know that we're using STATIC version of it's library
target_compile_definitions(RSE PRIVATE GLEW_STATIC)

# Link OpenGL Windows/Unix
find_package(OpenGL REQUIRED)
if (UNIX)
	find_package(GLUT REQUIRED)	
endif()
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(RSE ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# Add and link sdafx.h (stl pch)
set(PCH_HEADER ${PROJECT_SOURCE_DIR}/RSE/pch/sdafx.h)
target_precompile_headers(RSE PUBLIC ${PCH_HEADER})

include_directories(${PROJECT_SOURCE_DIR}/RSE/src/Core/include)
include_directories(${PROJECT_SOURCE_DIR}/RSE/src/Math/include)

target_include_directories(RSE PUBLIC include/)
target_include_directories(RSE PRIVATE ${SDL_INCLUDE_DIRS})
target_include_directories(RSE PRIVATE ${SDL_IMAGE_INCLUDE_DIRS})
target_include_directories(RSE PUBLIC ${FMT_INCLUDE_DIRS})
target_include_directories(RSE PRIVATE ${YAML_CPP_INCLUDE_DIRS})
#target_include_directories(RSE PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(RSE PUBLIC ${BOOST_FILESYSTEM_DIRS})
target_include_directories(RSE PUBLIC ${PROJECT_SOURCE_DIR}/RSE/pch/)
