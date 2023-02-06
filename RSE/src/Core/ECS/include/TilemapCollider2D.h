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
			for (const auto& tile_collider : m_TileColliders)
			{
				for (const auto& obj_collider : RSECore::colliders)
				{
					SDL_Rect r = tile_collider.SDL();
					Vector2f& obj_velocity = obj_collider->parent->GetComponent<TransformComponent>()->velocity;
					Vector2f& parent_pos = obj_collider->parent->GetComponent<TransformComponent>()->position;
					Vector2f origin = { obj_collider->colliderRect.x + obj_collider->colliderRect.w + 1, obj_collider->colliderRect.y };
					Vector2f cn_point;
					Vector2f cn_normal;
					float cn_fraction = 0.0f;
					Vector2f offset = { (cn_normal.x < 0) ? -16 : 0, (cn_normal.y < 0) ? -16 : 0 };
					/*if (Collision::SweptAABB(obj_collider->colliderRect, tile_collider, obj_velocity, cn_point, cn_normal, cn_fraction, deltaTime))
					{
						float remaining = 1.0f - cn_fraction;
						obj_velocity += cn_normal * Vector2f::Abs(obj_velocity) * remaining;
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