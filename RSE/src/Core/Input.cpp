#include "include/Input.h"
//#include "include/Vector2.h"

#include <iostream>

namespace RSE
{
	std::unordered_map<std::string, std::variant<KeyBind, AxisBind>> Input::m_Actions;
	std::unordered_map<uint32_t, bool> Input::keys;
	MouseState Input::mouse;

	int a[25];
}