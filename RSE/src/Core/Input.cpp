#include "include/Input.h"
//#include "include/Vector2f.h"

#include <iostream>

namespace Advres::RSE
{
	std::unordered_map<std::string, void*> Input::m_Actions;
	std::unordered_map<uint32_t, bool> Input::keys;
	MouseState Input::mouse;
}