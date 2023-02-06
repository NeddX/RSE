#ifndef NATIVE_BEHAVIOUR_COMPONENT_H
#define NATIVE_BEHAVIOUR_COMPONENT_H

#include "ECS.h"
#include "NativeBehaviour.h"

namespace Advres::RSE
{
	class NativeBehaviourComponent : public Component
	{
	public:
		NativeBehaviour* instance;

	public:
		std::function<void()> InstantiateInstance;
		std::function<void()> DisposeInstance;

	public:
		void Update(float deltaTime) override
		{
			if (instance) instance->Update(deltaTime);
		}
		void Render(float deltaTime) override
		{
			if (instance) instance->Render(deltaTime);
		}

	public:
		template<typename T>
		T* Bind()
		{
			InstantiateInstance = [&]()	{ T* instance = new T(); instance->m_Parent = this->parent; this->instance = (NativeBehaviour*) instance; };
			DisposeInstance		= [&]()	{ delete (T*) this->instance; this->instance = nullptr; };

			// TODO: This does not feel right
			InstantiateInstance();
			instance->Start();
			return (T*) instance;
		}
	};
}

#endif // NATIVE_BEHAVIOUR_COMPONENT_H