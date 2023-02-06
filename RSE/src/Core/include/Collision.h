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
			auto ac1Pos    = &col1->parent->GetComponent<TransformComponent>()->position;
			auto ac2Pos    = &col2->parent->GetComponent<TransformComponent>()->position;
			SDL_Rect rectA = { (int) ac1Pos->x, (int) ac1Pos->y, (int) col1->colliderRect.w, (int) col1->colliderRect.h };
			SDL_Rect rectB = { (int) ac2Pos->x, (int) ac2Pos->y, (int) col2->colliderRect.w, (int) col2->colliderRect.h };
			return AABB(&rectA, &rectB);
		}
		// TODO: Take Vecs as Consts!
		static inline bool RayVsRect(Vector2f& rayOrigin, Vector2f& rayDest, const Rect& target,
			Vector2f& contactPoint, Vector2f& contactNormal, float& contactFraction)
		{
			Vector2f distance = rayDest - rayOrigin;
			//SDL_RenderDrawLine(RSECore::sdlRenderer, (int)rayOrigin.x, (int)rayOrigin.y, (int)rayDest.x, (int)rayDest.y);
			
			float f_near_x = (target.x - rayOrigin.x) / distance.x;
			float f_far_x  = (target.x + target.w - rayOrigin.x) / distance.x;
			float f_near_y = (target.y - rayOrigin.y) / distance.y;
			float f_far_y  = (target.y + target.h - rayOrigin.y) / distance.y;

			if (f_near_x > f_far_x) std::swap(f_near_x, f_far_x);
			if (f_near_y > f_far_y) std::swap(f_near_y, f_far_y);

			// Check if the line intersects
			if (f_near_y > f_far_x || f_near_x > f_far_y || f_near_x > 1.0f || f_near_y > 1.0f) return false;

			contactFraction = std::max(f_near_x, f_near_y);
			float f_far  = std::min(f_far_x, f_far_y);
			
			// If the collision is happening on the opposite side of the rectangle
			if (f_far < 0) return false;

			contactPoint = rayOrigin + (distance * contactFraction);

			// Calculate the normal
			if (f_near_x > f_near_y) // Normal is on the X axis
				contactNormal.x = (distance.x < 0) ? 1.0f : -1.0f;
			else // Or else it is on the Y axis
				contactNormal.y = (distance.y < 0) ? 1.0f : -1.0f;
			/*
			if (distance.x < 0)
				contactNormal.x = 1.0f;
			else
				contactNormal.x = -1.0f;

			if (distance.y < 0)
				contactNormal.y = 1.0f;
			else
				contactNormal.y = -1.0f;*/

			return true;

			return true;
		}
		static inline bool SweptAABB(const Rect& b1, const Rect& b2, Vector2f& velocity, Vector2f& contactPoint, Vector2f& contactNormal, float& contactFraction, float& elapsedTime)
		{
			// TODO: Optimize!
			if (velocity == 0)
			{
				return false;
			}

			Rect expanded_rect = { b2.x - (b1.w / 2), b2.y - (b1.h / 2), b1.w + b2.w, b1.h + b2.h };
			RSECore::DrawDebugRect(expanded_rect);
			Vector2f ray_origin = { b1.x + b1.w / 2, b1.y + b1.h / 2 };
			Vector2f ray_dest = velocity * elapsedTime + ray_origin;
			RSECore::DrawDebugLine({ ray_origin, ray_dest });
			if (RayVsRect(ray_origin, ray_dest, expanded_rect, contactPoint, contactNormal, contactFraction))
			{
				if (contactFraction <= 1.0f)
					return true;
			}

			return false;
		}
		static Vector2f GetCollisionNormal(const SDL_Rect& rectA, const SDL_Rect& rectB)
		{
			Vector2f normal;
			Vector2f aCentrePoint = { (float)(rectA.x + (rectA.w / 2)), (float)(rectA.y + (rectA.h / 2)) };
			Vector2f bCentrePoint = { (float)(rectB.x + (rectB.w / 2)), (float)(rectB.y + (rectB.h / 2)) };
			Vector2f distance = bCentrePoint - aCentrePoint;
			Vector2f absDistance = Vector2f::Abs(distance);
			if (absDistance.x > absDistance.y) normal.x = (distance.x > 0) ? 1.0f : -1.0f; 
			else normal.y = (distance.y > 0) ? 1.0f : -1.0f;
			return normal;
		}

	public:
		void Update(float deltaTime);
	};
}

#endif // COLLISION_H