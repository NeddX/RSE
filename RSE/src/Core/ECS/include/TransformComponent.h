#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "ECS.h"
#include "../../include/Vector2.h"
#include "../../include/Transform.h"
#include "../../../Math/include/Mathf.h"

namespace Advres::RSE
{
	class TransformComponent : public Component
	{

	public:
		Vector2 position;
		Vector2 scale;
		float rotation;
		Vector2 velocity;

	public:
		TransformComponent(Transform transform = Transform()) : 
			position(transform.position), scale(transform.scale), rotation(transform.rotation)
		{
			if (this->scale == Vector2()) this->scale = { 1, 1 };
			velocity = { 0, 0 };
		}

	protected:
		void Update(float deltaTime) override
		{
			this->position.x += velocity.x;
			this->position.y += velocity.y;
		}

	public:
		void Serialize(YAML::Emitter& out) const
		{
			out << YAML::BeginMap;
			out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "position" << YAML::Value;
			Serializer::Serialize(position, out);
			out << YAML::Key << "scale" << YAML::Value;
			Serializer::Serialize(scale, out);
			out << YAML::Key << "rotation" << YAML::Value << rotation;
			out << YAML::Key << "velocity" << YAML::Value;
			Serializer::Serialize(velocity, out);
			out << YAML::EndMap;
			out << YAML::EndMap;
		}
	};
}

#endif // TRANSFORM_COMPONENT_H