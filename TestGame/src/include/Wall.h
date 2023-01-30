#ifndef WALL_H
#define WALL_H

#include <RSE.h>
#include <iostream>

using namespace Advres::RSE;

class Wall : public Behaviour
{
	void BoxCollider2D_OnEnter(const ComponentCollideResult2D result) override
	{
		fmt::println("Entered!");
	}
};

#endif // WALL_H