#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <RSE.h>

using namespace Advres::RSE;

class Projectile : public Behaviour
{
public:
	Vector2 velocity;
	TransformComponent* transform;

public:
	void Init() override
	{
		transform = this->parent->GetComponent<TransformComponent>();
	}
	void Update(float deltaTime) override
	{
		transform->position.x += velocity.x;
		transform->position.y += velocity.y;
	}
};

#endif // PROJECTILE_H