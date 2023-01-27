#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "ECS.h"
#include "../../include/Collision.h"

namespace Advres::RSE
{
	class Behaviour : public Component
	{
	private:
		std::string m_Script;

	public:
		virtual void BoxCollider2D_OnCollide(const ComponentCollideResult2D result)
		{

		}
		virtual void BoxCollider2D_OnEnter(const ComponentCollideResult2D result)
		{

		}
		virtual void BoxCollider2D_OnExit(const ComponentCollideResult2D result)
		{

		}

	public:
		void Serialize(YAML::Emitter& out) const override
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Behaviour" << YAML::Value;
			out << YAML::EndMap;
		}
	};
}

#endif // BEHVAIOUR_H