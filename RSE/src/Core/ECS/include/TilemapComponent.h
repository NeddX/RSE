#ifndef TILEMAP_COMPONENT_H
#define TILEMAP_COMPONENT_H

#include <sdafx.h>

#include "../../include/Core.h"
#include "../../include/Texture2D.h"
#include "../../include/Vector2.h"
#include "../../ECS/include/Components.h"

namespace Advres::RSE
{
	// Forward declerations
	class TilemapCollider2D;

	class TilemapComponent : public Component
	{
		friend class Serializer;
		friend class TilemapCollider2D;

	private:
		Texture2D* m_Texture;
		std::map<int, std::unordered_map<Vector2f, Vector2f>> m_Tiles;
		Vector2f m_GridSize;
		Vector2f m_TileSize;

	public:
		TilemapComponent(Texture2D* tileset, Vector2f gridSize, Vector2f tileSize) :
			m_Texture(tileset), m_GridSize(gridSize), m_TileSize(tileSize)
		{

		}

	public:
		void AddTile(Vector2f worldPos, int tileID, int layer = 0)
		{
			Vector2f vec;
			int columns = m_Texture->GetWidth() / (int) m_GridSize.x;
			int x = tileID % columns;
			int y = tileID / columns;
			vec.x = x * m_GridSize.x;
			vec.y = y * m_GridSize.y;
			m_Tiles[layer][Vector2f(worldPos.x, worldPos.y)] = vec;
		}
		void AddTile(Vector2f worldPos, Vector2f tilePos, int layer = 0)
		{
			//Vector2f vec;
			//vec.x = tilePos.x * m_GridSize.x;
			//vec.y = tilePos.y * m_GridSize.y;
			m_Tiles[layer][Vector2f(worldPos.x, worldPos.y)] = tilePos;
		}
		size_t GetTotalSize()
		{
			size_t total_size = m_Tiles.size();
			for (const auto& e : m_Tiles) total_size += e.second.size();
			return total_size;
		}
		void RemoveTile(Vector2f worldPos, int layer = 0)
		{
			m_Tiles[layer].erase(worldPos);
		}
		void Serialize(YAML::Emitter& out) const override
		{
			out << YAML::BeginMap;
			out << YAML::Key << "TilemapComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "m_Texture" << YAML::Value;
			Serializer::Serialize(Resources::GetResource<Texture2D>(m_Texture->GetID()).get(), out);
			out << YAML::Key << "m_GridSize" << YAML::Value;
			Serializer::Serialize(m_GridSize, out);
			out << YAML::Key << "m_TileSize" << YAML::Value;
			Serializer::Serialize(m_TileSize, out);
			out << YAML::Key << "m_Tiles" << YAML::Value;
			out << YAML::BeginMap;
			for (const auto& [outer_key, inner_map] : m_Tiles) 
			{
				out << YAML::Key << outer_key << YAML::Value;
				out << YAML::BeginSeq;
				for (const auto& [inner_key, inner_value] : inner_map) 
				{
					Serializer::Serialize(inner_key, out);
					Serializer::Serialize(inner_value, out);
				}
				out << YAML::EndSeq;
			}
			out << YAML::EndMap;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

	public:
					inline	size_t  GetSize()				noexcept { return m_Tiles.size(); }
					inline	Vector2f GetGridSize()			noexcept { return m_GridSize; }
					inline	Vector2f GetTileSize()			noexcept { return m_TileSize; }
		constexpr	inline	size_t  GetTilesetTextureID()	noexcept { return m_Texture->GetID(); }

	protected:
		void Render(float deltaTime) override
		{
			for (const auto& [key, value] : m_Tiles)
			{
				for (const auto& t : value)
				{
					SDL_Rect src_rect =  { (int) t.second.x,(int) t.second.y, (int) m_GridSize.x, (int) m_GridSize.y };
					SDL_Rect dest_rect = { (int) t.first.x, (int) t.first.y,  (int) m_TileSize.x, (int) m_TileSize.y };
					if (RSECore::IsInView(dest_rect)) RSECore::DrawTexture(m_Texture, &src_rect, &dest_rect);
				}
			}
		}
	};
}

#endif // TILEMAP_COMPONENT_H