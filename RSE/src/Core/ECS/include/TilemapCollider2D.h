#ifndef TILEMAP_COLLIDER_2D_H
#define TILEMAP_COLLIDER_2D_H

#include <sdafx.h>

#include "ECS.h"
#include "../../include/Core.h"
#include "../../include/Collision.h"


namespace Advres::RSE
{
	class TilemapCollider2D : public Component
	{
	private:
		int m_Layer;
		Vector2f m_TileSize;
		std::vector<Rect> m_TileColliders;
		TilemapComponent* m_Tilemap; 

	public:
		TilemapCollider2D(int layer = 0) : m_Layer(layer)
		{

		}

	public:
		void Init() override
		{
			m_Tilemap = parent->GetComponent<TilemapComponent>();
			m_TileSize = m_Tilemap->GetTileSize();

			auto& layer = m_Tilemap->m_Tiles[m_Layer];
			for (auto& t : layer)
			{
				auto found = std::find_if(m_TileColliders.begin(), m_TileColliders.end(), [&](const Rect r)
					{
						return t.first.x == r.x && t.first.y == r.y;
					});
				if (found != m_TileColliders.end())
				{
					fmt::println("Duplicate!");
					continue;
				}
				m_TileColliders.emplace_back((int) t.first.x, (int) t.first.y, (int) m_TileSize.x, (int) m_TileSize.y);
			}
		}
		void Update(float deltaTime) override
		{
			for (const auto& tc : m_TileColliders)
			{
				for (const auto& c : RSECore::colliders)
				{
					SDL_Rect r = tc.SDL();
					Vector2f parent_velocity = c->parent->GetComponent<TransformComponent>()->velocity;
					Vector2f& parent_pos = c->parent->GetComponent<TransformComponent>()->position;
					Vector2f origin = { c->colliderRect.x + c->colliderRect.w + 1, c->colliderRect.y };
					Vector2f cn_point;
					Vector2f cn_normal;
					float cn_fraction = 0.0f;
					Vector2f offset = { (cn_normal.x < 0) ? -16 : 0, (cn_normal.y < 0) ? -16 : 0 };
					//if (Collision::SweptAABB(c->colliderRect, tc, parent_velocity, cn_point, cn_normal, cn_fraction, deltaTime))
					//{
					//	fmt::println("HIT!");
					//	//parent_velocity = { 0, 0 };
					//}
					/*if (Collision::RayVsRect(origin, parent_velocity * deltaTime, tc.SDL(), cn_point, cn_normal, cn_fraction))
					{
						fmt::println("hit!");
					}*/
					/*if (Collision::AABB(&r, &c->colliderRect))
					{
						auto t = c->parent->GetComponent<TransformComponent>();
						Vector2f& pos = t->position;
						Vector2f norm = Collision::GetCollisionNormal(r, c->colliderRect);
						if (std::abs(norm.x) > std::abs(norm.y)) // X axis
						{
							if (norm.x > 0) // Right
							{
								int pixels = r.x + r.w - c->colliderRect.x;
								pos.x += pixels;
								//t->velocity.x = 0.01f;
							}
							else // Left
							{
								int pixels = c->colliderRect.x + c->colliderRect.w - r.x;
								pos.x -= pixels;
								//t->velocity.x = -0.01f;
							}
						}
						else // Y axis
						{
							// vertical
							if (norm.y > 0) // Down
							{
								int pixels = r.y + r.h - c->colliderRect.y;
								pos.y += pixels;
								//t->velocity.y = 0.01f;
							}
							else // Up
							{
								int pixels = c->colliderRect.y + c->colliderRect.h - r.y;
								pos.y -= pixels;
								//t->velocity.y = -0.01f;
							}
						}
					}*/
				}
			}
		}
		void Render(float deltaTime) override
		{
#ifdef _DEBUG
			for (const auto& r : m_TileColliders)
			{
				SDL_Rect sr = r.SDL();
				RSECore::DrawRect(sr);
				//Vector2f ds = { sr.x * 2, sr.y };
				//RSECore::DrawLine({ sr.x, sr.y }, ds);
			}
#endif
		}
	};
}

#endif // TILEMAP_COLLIDER_2D_H