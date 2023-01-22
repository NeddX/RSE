#ifndef COLLISION_H
#define COLLISION_H

#include <unordered_map>
#include <string>

#include "Vector2.h"
#include "../ECS/include/ECS.h"
#include "../ECS/include/BoxCollider2D.h"

namespace RSE
{
	struct ComponentCollideResult2D
	{
		Entity* entity;
		BoxCollider2D* boxCollider;
	};

	class Collision
	{
	public:
		static inline bool AABB(const SDL_Rect* rectA, const SDL_Rect* rectB)
		{
			return
				rectA->x + rectA->w >= rectB->x &&
				rectB->x + rectB->w >= rectA->x &&
				rectA->y + rectA->h >= rectB->y &&
				rectB->y + rectB->h >= rectA->y;
		}
		static inline bool AABB(const BoxCollider2D* col1, const BoxCollider2D* col2)
		{
			auto ac1Pos = &col1->entity->GetComponent<TransformComponent>()->position;
			auto ac2Pos = &col2->entity->GetComponent<TransformComponent>()->position;
			SDL_Rect rectA = { (int) ac1Pos->x, (int) ac1Pos->y, (int) col1->colliderRect.w, (int) col1->colliderRect.h };
			SDL_Rect rectB = { (int) ac2Pos->x, (int) ac2Pos->y, (int) col2->colliderRect.w, (int) col2->colliderRect.h };
			return AABB(&rectA, &rectB);
		}

	public:
		void Update(float deltaTime);
	};
}

#endif // COLLISION_H