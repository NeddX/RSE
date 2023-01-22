#ifndef BOX_COLLIDER_2D_H
#define BOX_COLLIDER_2D_H

#include <SDL.h>
#include <string>
#include <yaml-cpp/yaml.h>
#include <algorithm>

#include "ECS.h"
#include "TransformComponent.h"
#include "../../include/Core.h"
#include "../../include/Transform.h"
#include "../../include/Serializer.h"

namespace RSE
{
	// Forward declerations
	class Behaviour;
	struct ComponentCollideResult2D;

	typedef void (Behaviour::* BehaviourCallbackMethod)(ComponentCollideResult2D);

	class BoxCollider2D : public Component
	{
		friend class Serializer;

	private:
		std::string m_Tag;
		TransformComponent* m_ActorTransform;
		Behaviour* m_BehaviourObj;
	
	public:
		SDL_Rect colliderRect;
		Vector2 colliderExtent;
		Transform transform;
		bool trigger;

	protected:
		bool m_IsColliding;

	public:
		BehaviourCallbackMethod OnCollide;
		BehaviourCallbackMethod OnEnter;
		BehaviourCallbackMethod OnExit;

	public:
		BoxCollider2D(const std::string& tag, Transform transform = Transform(), Vector2 colliderExtent = {32, 32});
		~BoxCollider2D();

	public:
		void Block();

	public:
		virtual void Init() override;
		virtual void Update(float deltaTime) override;
		virtual void Render(float deltaTime) override;
		
	public:
		std::string GetTag() const;

	public:
		template<typename CallbackMethod, typename ArgType>
		inline void CallBehaviourCallback(CallbackMethod method, ArgType arg)
		{
			(m_BehaviourObj->*method)(arg);
		}
		inline void SetBehaviourObject(Behaviour* object)
		{
			m_BehaviourObj = object;
		}
		void Serialize(YAML::Emitter& out) const
		{
			out << YAML::BeginMap;
			out << YAML::Key << "BoxCollider2D" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "m_Tag" << YAML::Value << m_Tag;
			out << YAML::Key << "m_ActorTransform" << YAML::Value << "owner.GetComponent<TransformComponent>";
			out << YAML::Key << "m_BehaviourObj" << YAML::Value << (uint64_t) m_BehaviourObj;
			out << YAML::Key << "colliderRect" << YAML::Value;
			Serializer::Serialize(colliderRect, out);
			out << YAML::Key << "colliderExtent" << YAML::Value;
			Serializer::Serialize(colliderExtent, out);
			out << YAML::Key << "transform" << YAML::Value;
			Serializer::Serialize(transform, out);
			out << YAML::Key << "trigger" << YAML::Value << trigger;
			out << YAML::Key << "OnCollide" << YAML::Value << OnCollide;
			out << YAML::Key << "OnEnter" << YAML::Value << OnEnter;
			out << YAML::Key << "OnExit" << YAML::Value << OnExit;
			out << YAML::EndMap;
			out << YAML::EndMap;
		}
	};
}

#endif // BOX_COLLIDER_2D_H