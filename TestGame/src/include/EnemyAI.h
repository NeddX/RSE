#ifndef ENEMYAI_H
#define ENEMYAI_H

#include <RSE.h>
#include <memory>

using namespace Advres::RSE;

class EnemyAI : public Behaviour
{
private:
	Entity* player;
	Entity* debugRect;
	TransformComponent* playerTransform;
	TransformComponent* transform;
	TransformComponent* projTransComp;
	Sprite* sprite;
	BoxCollider2D* collider;
	Vector2f start;
	Vector2f input;
	int speed = 20;
	std::chrono::steady_clock::time_point sizeTp;
	float dist = 0.1;
	Vector2f prevPlayerPos;

public:
	void Init() override
	{
		player = RSECore::GetEntityByTag("player");
		playerTransform = player->GetComponent<TransformComponent>();
		transform = this->parent->GetComponent<TransformComponent>();
		start = transform->position;
		//debugRect = RSECore::AddEntity();
		//debugRect->AddComponent<TransformComponent>();
		//debugRect->AddComponent<BoxCollider2D>("debug", Transform(), Vector2f(32, 32))->trigger = true;
	} 

	void Update(float deltaTime) override
	{
		//dist += 0.0005;
		//dist = Mathf::Clamp(dist, 0.0f, 1.0f);

		/*dist = 0.5;

		float distance = playerTransform->position.DistanceTo(transform->position);
		Vector2f lerped = transform->position.Lerp(playerTransform->position, dist);

		if (distance > 40)
		{
			debugRect->GetComponent<TransformComponent>()->position = lerped;
		}

		prevPlayerPos = playerTransform->position;
		*/

		//std::cout << "Start: " << start << "\nPlayer: " << playerTransform->position << std::endl;
		//std::cout << "Lerp: " << lerped << std::endl << std::endl;
	}
};

#endif // ENEMYAI_H