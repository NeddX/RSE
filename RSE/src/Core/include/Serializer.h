#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <sdafx.h>

#include "Vector2.h"
#include "Transform.h"

namespace Advres::RSE
{
	// Forward declerations
	class Entity;
	class EntityManager;
	class Texture2D;
	class Sprite;
	class BoxCollider2D;
	class TilemapComponent;

	class Serializer
	{
	public:
		static void Serialize(const Vector2& vec, YAML::Emitter& out);
		static void Serialize(const Transform& trans, YAML::Emitter& out);
		static void Serialize(const SDL_Rect& rect, YAML::Emitter& out);
		static void Serialize(const Entity* parent, YAML::Emitter& out);
		static void Serialize(Texture2D* texture, YAML::Emitter& out);
		static void SerializeScene(const char* file, const EntityManager* mgr);

	public:
		static void Deserialize(const YAML::Node& node, Vector2& vec);
		static void Deserialize(const YAML::Node& node, Transform& trans);
		static void Deserialize(const YAML::Node& node, SDL_Rect& rect);
		static void Deserialize(const YAML::Node& node, Texture2D* texture);
		static void Deserialize(const YAML::Node& node, Sprite* sprite);
		static void Deserialize(const YAML::Node& node, BoxCollider2D* collider);
		static void Deserialize(const YAML::Node& node, TilemapComponent* tilemap);
		static void Deserialize(const YAML::Node& node, Entity* parent);
		static void Deserialize(const char* file, EntityManager* actorMgr);
	};
}

#endif // SERIALIZER_H