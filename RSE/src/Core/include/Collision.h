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
		// TODO: Take Vecs as Consts!
		static inline bool RayVsRect(Vector2f& rayOrigin, Vector2f& rayDest, const Rect& rect,
			Vector2f& contactPoint, Vector2f& contactNormal, float& contactFraction)
		{
			Vector2f distance = rayDest - rayOrigin;
			//SDL_RenderDrawLine(RSECore::sdlRenderer, (int)rayOrigin.x, (int)rayOrigin.y, (int)rayDest.x, (int)rayDest.y);
			
			float t_x_near = (rect.x - rayOrigin.x) / distance.x;
			float t_x_far = ((rect.x + rect.w) - rayOrigin.x) / distance.x;
			float t_y_near = (rect.y - rayOrigin.y) / distance.y;
			float t_y_far = ((rect.y + rect.h) - rayOrigin.y) / distance.y;

			if (t_x_near > t_x_far) std::swap(t_x_near, t_x_far);
			if (t_y_near > t_y_far) std::swap(t_y_near, t_y_far);

			// Check if the line intersects
			if (t_y_near > t_x_far || t_x_near > t_y_far || t_x_near > 1.0f || t_y_near > 1.0f) return false;

			float t_near = std::max(t_x_near, t_y_near);
			float t_far = std::min(t_x_far, t_y_far);
			
			// If the collision is happening on the opposite side of the rectangle
			if (t_far < 0) return false;

			contactPoint = rayOrigin + (distance * t_near);

			// Calculate the normal
			if (t_x_near > t_y_near) // Normal is on the X axis
				contactNormal.x = (distance.x < 0) ? 1.0f : -1.0f;
			else // Or else it is on the Y axis
				contactNormal.y = (distance.y < 0) ? 1.0f : -1.0f;

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
			//RSECore::DrawDebugRect(expanded_rect);
			Vector2f ray_origin = { b1.x + b1.w / 2, b1.y + b1.h / 2 };
			Vector2f ray_dest = velocity * elapsedTime + ray_origin;
			//RSECore::DrawDebugLine({ ray_origin, ray_dest });
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