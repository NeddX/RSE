
#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "ECS.h"
#include "../../include/Collision.h"

namespace Advres::RSE
{
	class NativeBehaviourComponent;

	class NativeBehaviour
	{
		friend class NativeBehaviourComponent;

	public:
		Entity* m_Parent;

	public:
		virtual ~NativeBehaviour()
		{

		}

	public:
		virtual void Start()
		{

		}
		virtual void Update(float deltaTime)
		{

		}
		virtual void Render(float deltaTime)
		{

		}

	public:
		template<typename T, typename... TArgs>
		T* AddComponent(TArgs&& ...args)
		{
			return m_Parent->AddComponent<T>(std::forward<TArgs>(args)...);
		}
		template<typename T>
		T* GetComponent()
		{
			return m_Parent->GetComponent<T>();
		}
		template<typename T>
		bool HasComponent()
		{
			return m_Parent->HasComponent<T>();
		}
		Group GetGroup()
		{
			return m_Parent->GetGroup();
		}
		void SetGroup(Group group)
		{
			return m_Parent->SetGroup(group);
		}
		bool IsActive()
		{
			return m_Parent->IsActive();
		}
	};
}

#endif // BEHVAIOUR_H