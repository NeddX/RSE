#include "include/Serializer.h"
#include "../ECS/include/Components.h"
#include "include/ResourceHandler.h"
#include "include/Texture2D.h"

namespace RSE
{
	void Serializer::Serialize(const Vector2& vec, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Vector2" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "x" << YAML::Value << vec.x;
		out << YAML::Key << "y" << YAML::Value << vec.y;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

    void Serializer::Serialize(const Transform& trans, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "position" << YAML::Value;
		Serialize(trans.position, out);
		out << YAML::Key << "scale" << YAML::Value;
		Serialize(trans.scale, out);
		out << YAML::Key << "rotation" << YAML::Value << trans.rotation;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	void Serializer::Serialize(const SDL_Rect& rect, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SDL_Rect" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "x" << YAML::Value << rect.x;
		out << YAML::Key << "y" << YAML::Value << rect.y;
		out << YAML::Key << "w" << YAML::Value << rect.w;
		out << YAML::Key << "h" << YAML::Value << rect.h;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	void Serializer::Serialize(const Entity* entity, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "tag" << YAML::Value << entity->tag;
		out << YAML::Key << "render" << YAML::Value << entity->render;
		out << YAML::Key << "group" << YAML::Value << entity->m_Group;
		out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;
		for (auto const& c : entity->m_Components) c->Serialize(out);
		out << YAML::EndSeq;
		out << YAML::EndMap;
	}

	void Serializer::Serialize(Texture2D* texture, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "TextureResource" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "ResourceID" << YAML::Value << texture->GetID();
		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	
	void Serializer::SerializeScene(const char* file, const EntityManager* mgr)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Actors";
		out << YAML::Value << YAML::BeginSeq;
		for (const auto& e : mgr->m_Entities) Serialize(e.get(), out);
		out << YAML::EndSeq;

		std::ofstream fs(file);
		if (fs.is_open() && fs.good()) fs.write(out.c_str(), strlen(out.c_str()));
		fs.close();
	}

	void Serializer::Deserialize(const YAML::Node& node, Vector2& vec)
	{
		vec.x = node["Vector2"]["x"].as<float>(); 
		vec.y = node["Vector2"]["y"].as<float>();
	}

	void Serializer::Deserialize(const YAML::Node& node, Transform& trans)
	{
		auto& root = node["Transform"];
		Deserialize(root["position"], trans.position);
		Deserialize(root["scale"], trans.scale);
		trans.rotation = root["rotation"].as<float>();
	}

	void Serializer::Deserialize(const YAML::Node& node, SDL_Rect& rect)
	{
		auto& root = node["SDL_Rect"];
		rect.x = root["x"].as<int>();
		rect.y = root["y"].as<int>();
		rect.w = root["w"].as<int>();
		rect.h = root["h"].as<int>();
	}

	void Serializer::Deserialize(const YAML::Node& node, Texture2D* texture)
	{
		auto& root = node["TextureResource"];
		size_t id = root["ResourceID"].as<size_t>();
		return;
	}

	void Serializer::Deserialize(const YAML::Node& node, Sprite* sprite)
	{
		auto& root = node["SpriteComponent"];
		sprite->actorTransform = sprite->entity->GetComponent<TransformComponent>();
		Deserialize(root["transform"], sprite->transform);
		size_t id = root["texture"]["TextureResource"]["ResourceID"].as<size_t>();
		sprite->texture = Resources::GetResource<Texture2D>(id).get();
		Deserialize(root["m_SrcRect"], sprite->m_SrcRect);
		Deserialize(root["m_DestRect"], sprite->m_DestRect);
		Deserialize(root["m_CurrentDestRect"], sprite->m_CurrentDestRect);
	}

	void Serializer::Deserialize(const YAML::Node& node, BoxCollider2D* collider)
	{
		auto& root = node["BoxCollider2D"];
		collider->m_Tag = root["m_Tag"].as<std::string>();
		collider->m_ActorTransform = collider->entity->GetComponent<TransformComponent>();
		Deserialize(root["colliderRect"], collider->colliderRect);
		Deserialize(root["colliderExtent"], collider->colliderExtent);
		Deserialize(root["transform"], collider->transform);
		collider->trigger = root["trigger"].as<bool>();
	}

	void Serializer::Deserialize(const YAML::Node& node, TileMapComponent* tilemap)
	{
		auto& root = node["TileMapComponent"];
		size_t id = root["m_Texture"]["TextureResource"]["ResourceID"].as<size_t>();
		tilemap->m_Texture = Resources::GetResource<Texture2D>(id).get();
		Deserialize(root["m_GridSize"], tilemap->m_GridSize);
		Deserialize(root["m_TileSize"], tilemap->m_TileSize);
		YAML::Node tilesNode = root["m_Tiles"];
		for (const auto& t : tilesNode)
		{
			for (auto it = t.second.begin(); it != t.second.end(); ++it)
			{
				Vector2 worldPos;
				Vector2 tilePos;
				Deserialize(*it, worldPos);
				Deserialize(*++it, tilePos);
				tilemap->m_Tiles[t.first.as<int>()][worldPos] = tilePos;
			}
		}
	}

	void Serializer::Deserialize(const YAML::Node& node, Entity* entity)
	{
		entity->tag = node["tag"].as<std::string>();
		entity->m_Group = node["group"].as<size_t>();
		entity->render = node["render"].as<bool>();

		if (entity->m_Group != -1)
		{
			entity->AddToGroup(entity->m_Group);
		}

		// Do not initialize deserialized objects!
		YAML::Node componentsNode = node["Components"];
		for (const auto& c : componentsNode)
		{
			for (const auto& pair : c)
			{
				auto& currentNode = pair.second;
				std::string	typeName = pair.first.as<std::string>();
				if (typeName == "TransformComponent")
				{
					TransformComponent* trans = new TransformComponent();
					trans->entity = entity;
					Deserialize(currentNode["position"], trans->position);
					Deserialize(currentNode["scale"], trans->scale);
					trans->rotation = currentNode["rotation"].as<float>();
					Deserialize(currentNode["velocity"], trans->velocity);
					entity->AddComponent(trans, false);
				}
				else if (typeName == "SpriteComponent")
				{
					Sprite* sprite = new Sprite(nullptr, Transform());
					sprite->entity = entity;
					Deserialize(c, sprite);
					entity->AddComponent(sprite, false);
				}
				else if (typeName == "BoxCollider2D")
				{
					BoxCollider2D* collider = new BoxCollider2D("", Transform());
					collider->entity = entity;
					Deserialize(c, collider);
					entity->AddComponent(collider, false);
				}
				else if (typeName == "TileMapComponent")
				{
					TileMapComponent* tilemap = new TileMapComponent(nullptr, Vector2(), Vector2());
					tilemap->entity = entity;
					Deserialize(c, tilemap);
					entity->AddComponent(tilemap, false);
				}
			}
		}
	}

	void Serializer::Deserialize(const char* file, EntityManager* actorMgr)
	{
		size_t size = xstd::File::FileSize(file);
		std::ifstream fs(file);
		/*if (fs.is_open())
		{
			char* str = new char[size + 1];
			memset(str, 0, size + 1);
			fs.read(str, size);
			fs.close();
			char* decStr = xstd::File::DecodeBHS8ToStr(str);
			YAML::Node node = YAML::Load(decStr);

			YAML::Node actorsNode = node["Actors"];
			for (const auto& e : actorsNode)
			{
				std::shared_ptr<Entity> entity = actorMgr->AddEntity();
				Deserialize(e, entity);
			}
			delete str;
			delete decStr;
		}*/
		if (fs.is_open())
		{
			std::string out((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
			fs.close();

			std::stringstream ss(out.c_str());
			YAML::Node node = YAML::Load(ss);

			YAML::Node actorsNode = node["Actors"];
			for (const auto& e : actorsNode)
			{
				Entity* entity = actorMgr->AddEntity();
				Deserialize(e, entity);
			}
		}
	}
}