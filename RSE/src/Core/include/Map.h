#ifndef MAP_H
#define MAP_H

#include <fstream>

#include "Vector2.h"
#include "Texture2D.h"

namespace Advres::RSE
{
	class Map
	{
	public:
		void LoadMap(const char* file, Vector2 size);
	};
}

#endif // MAP_H