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
		std::map<int, std::unordered_map<Vector2, Vector2>> m_Tiles;
		Vector2 m_GridSize;
		Vector2 m_TileSize;

	public:
		TilemapComponent(Texture2D* tileset, Vector2 gridSize, Vector2 tileSize) :
			m_Texture(tileset), m_GridSize(gridSize), m_TileSize(tileSize)
		{

		}

	public:
		void AddTile(Vector2 worldPos, int tileID, int layer = 0)
		{
			Vector2 vec;
			int columns = m_Texture->GetWidth() / (int) m_GridSize.x;
			int x = tileID % columns;
			int y = tileID / columns;
			vec.x = x * m_GridSize.x;
			vec.y = y * m_GridSize.y;
			m_Tiles[layer][Vector2(worldPos.x, worldPos.y)] = vec;
		}
		void AddTile(Vector2 worldPos, Vector2 tilePos, int layer = 0)
		{
			//Vector2 vec;
			//vec.x = tilePos.x * m_GridSize.x;
			//vec.y = tilePos.y * m_GridSize.y;
			m_Tiles[layer][Vector2(worldPos.x, worldPos.y)] = tilePos;
		}
		size_t GetTotalSize()
		{
			size_t totalSize = m_Tiles.size();
			for (const auto& e : m_Tiles) totalSize += e.second.size();
			return totalSize;
		}
		void RemoveTile(Vector2 worldPos, int layer = 0)
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
			for (const auto& [outerKey, innerMap] : m_Tiles) 
			{
				out << YAML::Key << outerKey << YAML::Value;
				out << YAML::BeginSeq;
				for (const auto& [innerKey, innerValue] : innerMap) 
				{
					Serializer::Serialize(innerKey, out);
					Serializer::Serialize(innerValue, out);
				}
				out << YAML::EndSeq;
			}
			out << YAML::EndMap;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

	public:
					inline	size_t  GetSize()				noexcept { return m_Tiles.size(); }
					inline	Vector2 GetGridSize()			noexcept { return m_GridSize; }
					inline	Vector2 GetTileSize()			noexcept { return m_TileSize; }
		constexpr	inline	size_t  GetTilesetTextureID()	noexcept { return m_Texture->GetID(); }

	protected:
		void Render(float deltaTime) override
		{
			for (const auto& [key, value] : m_Tiles)
			{
				for (const auto& t : value)
				{
					SDL_Rect srcRect =  { (int) t.second.x,(int) t.second.y, (int) m_GridSize.x, (int) m_GridSize.y };
					SDL_Rect destRect = { (int) t.first.x, (int) t.first.y,  (int) m_TileSize.x, (int) m_TileSize.y };
					if (RSECore::IsInView(destRect)) RSECore::DrawTexture(m_Texture, &srcRect, &destRect);
				}
			}
		}
	};
}

#endif // TILEMAP_COMPONENT_H