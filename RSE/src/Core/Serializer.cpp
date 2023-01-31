#include "include/Serializer.h"
#include "../ECS/include/Components.h"
#include "include/ResourceHandler.h"
#include "include/Texture2D.h"

namespace Advres::RSE
{
	void Serializer::Serialize(const Vector2f& vec, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Vector2f" << YAML::Value;
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

	void Serializer::Serialize(const Entity* parent, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "tag" << YAML::Value << parent->tag;
		out << YAML::Key << "render" << YAML::Value << parent->render;
		out << YAML::Key << "group" << YAML::Value << parent->m_Group;
		out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;
		for (auto const& c : parent->m_Components) c->Serialize(out);
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

	void Serializer::Deserialize(const YAML::Node& node, Vector2f& vec)
	{
		vec.x = node["Vector2f"]["x"].as<float>(); 
		vec.y = node["Vector2f"]["y"].as<float>();
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
		sprite->actorTransform = sprite->parent->GetComponent<TransformComponent>();
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
		collider->m_ActorTransform = collider->parent->GetComponent<TransformComponent>();
		Deserialize(root["colliderRect"], collider->colliderRect);
		Deserialize(root["colliderExtent"], collider->colliderExtent);
		Deserialize(root["transform"], collider->transform);
		collider->trigger = root["trigger"].as<bool>();
	}

	void Serializer::Deserialize(const YAML::Node& node, TilemapComponent* tilemap)
	{
		auto& root = node["TilemapComponent"];
		size_t id = root["m_Texture"]["TextureResource"]["ResourceID"].as<size_t>();
		tilemap->m_Texture = Resources::GetResource<Texture2D>(id).get();
		Deserialize(root["m_GridSize"], tilemap->m_GridSize);
		Deserialize(root["m_TileSize"], tilemap->m_TileSize);
		YAML::Node tilesNode = root["m_Tiles"];
		for (const auto& t : tilesNode)
		{
			for (auto it = t.second.begin(); it != t.second.end(); ++it)
			{
				Vector2f worldPos;
				Vector2f tilePos;
				Deserialize(*it, worldPos);
				Deserialize(*++it, tilePos);
				tilemap->m_Tiles[t.first.as<int>()][worldPos] = tilePos;
			}
		}
	}

	void Serializer::Deserialize(const YAML::Node& node, Entity* parent)
	{
		parent->tag = node["tag"].as<std::string>();
		parent->m_Group = node["group"].as<size_t>();
		parent->render = node["render"].as<bool>();

		/*if (parent->m_Group != -1)
		{
			parent->AddToGroup(parent->m_Group);
		}*/

		// Do not initialize deserialized objects!
		YAML::Node components_node = node["Components"];
		for (const auto& c : components_node)
		{
			for (const auto& pair : c)
			{
				auto& current_node = pair.second;
				std::string	type_name = pair.first.as<std::string>();
				if (type_name == "TransformComponent")
				{
					TransformComponent* trans = new TransformComponent();
					trans->parent = parent;
					Deserialize(current_node["position"], trans->position);
					Deserialize(current_node["scale"], trans->scale);
					trans->rotation = current_node["rotation"].as<float>();
					Deserialize(current_node["velocity"], trans->velocity);
					parent->AddComponent(trans, false);
				}
				else if (type_name == "SpriteComponent")
				{
					Sprite* sprite = new Sprite(nullptr, Transform());
					sprite->parent = parent;
					Deserialize(c, sprite);
					parent->AddComponent(sprite, false);
				}
				else if (type_name == "BoxCollider2D")
				{
					BoxCollider2D* collider = new BoxCollider2D("", Transform());
					collider->parent = parent;
					Deserialize(c, collider);
					parent->AddComponent(collider, false);
				}
				else if (type_name == "TilemapComponent")
				{
					TilemapComponent* tilemap = new TilemapComponent(nullptr, Vector2f(), Vector2f());
					tilemap->parent = parent;
					Deserialize(c, tilemap);
					parent->AddComponent(tilemap, false);
				}
			}
		}
	}

	void Serializer::Deserialize(const char* file, EntityManager* actorMgr)
	{
		size_t size = xstdf::file::FileSize(file);
		std::ifstream fs(file);
		/*if (fs.is_open())
		{
			char* str = new char[size + 1];
			memset(str, 0, size + 1);
			fs.read(str, size);
			fs.close();
			char* decStr = xstdf::File::DecodeBHS8ToStr(str);
			YAML::Node node = YAML::Load(decStr);

			YAML::Node actors_node = node["Actors"];
			for (const auto& e : actors_node)
			{
				std::shared_ptr<Entity> parent = actorMgr->AddEntity();
				Deserialize(e, parent);
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

			YAML::Node actors_node = node["Actors"];
			for (const auto& e : actors_node)
			{
				Entity* parent = actorMgr->AddEntity();
				Deserialize(e, parent);
			}
		}
	}
}