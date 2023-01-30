#ifndef PCH_H
#define PCH_H

// STL Headers
#include <iostream>
#include <memory>
#include <cstdint>
#include <chrono>
#include <thread>
#include <fstream>
#include <algorithm>
#include <utility>
#include <functional>
#include <type_traits>
#include <cstring>
#include <mutex>
#include <cstdio>

// STL Containers
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <array>
#include <initializer_list>

// Glew
//#include <GL/glew.h>
//#include <GL/glu.h>

// Platform specific
#ifdef __APPLE__
    #include <unistd.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
#ifdef _WIN32
    #define NOMINMAX // fucking windows being windows
    #include <windows.h>
    #include <corecrt_io.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#else
    #include <unistd.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif
#endif

// Library Specific
#include <yaml-cpp/yaml.h>
#include <fmt/core.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

// Project Specific
#include "../src/Util/xstdf.h"

#endif // PCH_H