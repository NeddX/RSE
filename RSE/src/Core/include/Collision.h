#ifndef COLLISION_H
#define COLLISION_H

#include <unordered_map>
#include <string>

#include "Vector2.h"
#include "../ECS/include/ECS.h"
#include "../ECS/include/BoxCollider2D.h"
#include "../include/Core.h"

namespace Advres::RSE
{
	struct ComponentCollideResult2D
	{
		Entity* parent;
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
			auto ac1Pos = &col1->parent->GetComponent<TransformComponent>()->position;
			auto ac2Pos = &col2->parent->GetComponent<TransformComponent>()->position;
			SDL_Rect rectA = { (int) ac1Pos->x, (int) ac1Pos->y, (int) col1->colliderRect.w, (int) col1->colliderRect.h };
			SDL_Rect rectB = { (int) ac2Pos->x, (int) ac2Pos->y, (int) col2->colliderRect.w, (int) col2->colliderRect.h };
			return AABB(&rectA, &rectB);
		}
		static inline bool RayVsRect(const Vector2& rayOrigin, const Vector2& rayDest, const Rect& rect)
		{
			Vector2 dir = { rayDest.x - rayOrigin.x, rayDest.y - rayOrigin.y };

			// Compute the minimum and maximum values for each axis
			float t_x_min = (rect.x - rayOrigin.x) / dir.x;
			float t_x_max = (rect.x + rect.w - rayOrigin.x) / dir.x;
			if (t_x_min > t_x_max) std::swap(t_x_min, t_x_max);

			float t_y_min = (rect.y - rayOrigin.y) / dir.y;
			float t_y_max = (rect.y + rect.h - rayOrigin.y) / dir.y;
			if (t_y_min > t_y_max) std::swap(t_y_min, t_y_max);

			// Check if the ray intersects the AABB
			if ((t_x_min > t_y_max) || (t_y_min > t_x_max)) return false;

			if (t_y_min > t_x_min) t_x_min = t_y_min;
			if (t_y_max < t_x_max) t_x_max = t_y_max;

			return (t_x_min < 1) && (t_x_max > 0);
		}
		static inline float SweptAABB(const Rect& b1, const Rect& b2, Vector2& normals)
		{
			return 1;
		}
		static Vector2 GetCollisionNormal(const SDL_Rect& rectA, const SDL_Rect& rectB)
		{
			Vector2 normal;
			Vector2 aCentrePoint = { (float)(rectA.x + (rectA.w / 2)), (float)(rectA.y + (rectA.h / 2)) };
			Vector2 bCentrePoint = { (float)(rectB.x + (rectB.w / 2)), (float)(rectB.y + (rectB.h / 2)) };
			Vector2 distance = bCentrePoint - aCentrePoint;
			Vector2 absDistance = Vector2::Abs(distance);
			if (absDistance.x > absDistance.y) normal.x = (distance.x > 0) ? 1.0f : -1.0f; 
			else normal.y = (distance.y > 0) ? 1.0f : -1.0f;
			return normal;
		}

	public:
		void Update(float deltaTime);
	};
}

#endif // COLLISION_H