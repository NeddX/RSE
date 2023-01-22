#ifndef WALL_H
#define WALL_H

#include <RSE.h>
#include <iostream>

using namespace RSE;

class Wall : public Behaviour
{
	void BoxCollider2D_OnEnter(const ComponentCollideResult2D result) override
	{
		std::cout << "Entered!" << std::endl;
	}
};

#endif // WALL_H