#ifndef PCH_H
#define PCH_H

// STL Headers
#include <iostream>
#include <memory>
#include <cstdint>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <utility>
#include <functional>
#include <format>
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

// Platform specific
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Library Specific
#include <yaml-cpp/yaml.h>
#include <fmt/core.h>
#include <SDL.h>
#include <SDL_image.h>

// Project Specific
#include "../src/Util/xstdf.h"

#endif // PCH_H