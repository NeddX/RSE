#ifndef ECS_H
#define ECS_H

#include <sdafx.h>

#include "../../include/Serializer.h"

namespace Advres::RSE
{
	// Forward declerations
	class Entity;
	class Component;
	class EntityManager;

	constexpr size_t maxComponenets = 32;
	constexpr size_t maxGroups = 32;

	typedef size_t ComponentID;
	typedef std::bitset<maxComponenets> ComponenetBitSet;
	typedef std::bitset<maxGroups> GroupBitSet;
	typedef std::array<Component*, maxComponenets> ComponentList;
	typedef size_t Group;

	inline ComponentID NewComponentTypeID() noexcept
	{
		static ComponentID last_id = 0;
		return last_id++;
	}

	template<typename T>
	inline ComponentID GetComponentTypeID() noexcept
	{
		static ComponentID type_id = NewComponentTypeID();
		return type_id;
	}

	class Component
	{
		friend class Serializer;
		friend class Entity;

	public:
		Entity* parent;
		bool render = true;
		bool active = true;

	public:
		virtual ~Component()
		{
			fmt::println("Component Destroyed!");
		}

	public:
		virtual void Init()					 {};
		virtual void Update(float deltaTime) {};
		virtual void Render(float deltaTime) {};

	public:
		virtual void Serialize(YAML::Emitter& out) const
		{
			out << YAML::BeginMap;
			out << YAML::Key << "IBaseComponentClass" << YAML::Value;
			out << YAML::EndMap;
		}
	};

	class Entity
	{
		friend class Serializer;
		friend class EntityManager;

	public:
		std::string tag;
		bool render = true;
		bool active = true;

	private:
		std::vector<Component*> m_Components;
		EntityManager& m_Manager;
		ComponentList m_ComponentList;
		ComponenetBitSet m_ComponentBitSet;
		GroupBitSet m_GroupBitSet;
		Group m_Group = -1;
		
	protected:
		bool m_DidInit = false;

	public:
		Entity(EntityManager& mgrRef) : m_Manager(mgrRef)
		{
			memset(m_ComponentList.data(), 0, maxComponenets * sizeof(Component*));
		}
		~Entity()
		{
			for (auto& c : m_Components) delete c;
			fmt::println("Entity Destroyed!");
		}

	public:
		void Start();
		void Update(float deltaTime)
		{
			for (auto& c : m_Components) 
				c->Update(deltaTime);
		}
		void Render(float deltaTime) 
		{
			if (render) for (auto& c : m_Components) if (c->render) c->Render(deltaTime);
		}

	public:
		inline void Dispose()
		{
			active = false;
		}
		inline bool IsActive() const
		{
			return active;
		}
		inline bool IsInGroup(Group group)
		{
			return m_GroupBitSet[group];
		}
		inline Group GetGroup()
		{
			return m_Group;
		}
		void SetGroup(Group group);

	public:
		template<typename T>
		inline bool HasComponent() const
		{
			return m_ComponentBitSet[GetComponentTypeID<T>()];
		}
		template<typename T, typename... TArgs>
		T* AddComponent(TArgs&&... args)
		{
			T* c = new T(std::forward<TArgs>(args)...);
			c->parent = this;
			m_Components.push_back(c);

			m_ComponentList[GetComponentTypeID<T>()] = c;
			m_ComponentBitSet[GetComponentTypeID<T>()] = true;

			c->Init();
			return c;
		}
		template<typename T>
		T* AddComponent(T* c, bool init = true)
		{
			c->parent = this;
			m_Components.push_back(c);
			
			m_ComponentList[GetComponentTypeID<T>()] = c;
			m_ComponentBitSet[GetComponentTypeID<T>()] = true;

			if (init) c->Init();
			return c;
		}
		template<typename T>
		T* GetComponent() const
		{
			ComponentID id = GetComponentTypeID<T>();
  			T* ptr = (T*) m_ComponentList[id];
			return ptr;
		}
	};

	class EntityManager
	{
		friend class Serializer;

	private:
		std::vector<std::shared_ptr<Entity>> m_Entities;
		std::array<std::vector<Entity*>, maxGroups> m_Groups;

	public:
		void Update(float deltaTime)
		{
			for (auto& e : m_Entities)
			{
				if (e->active)
				{
					e->Update(deltaTime);
				}
			}
		}
		void Render(float deltaTime)
		{
			for (auto& g : m_Groups) for (auto& e : g) if (e->render) e->Render(deltaTime);
			//for (auto& e : m_Entities) e->Render(deltaTime);
		}
		void Refresh()
		{
			/*for (int i = 0; i < maxGroups; ++i)
			{
				auto& v = m_Groups[i];
				v.erase(std::remove_if(v.begin(), v.end(), [i](Entity* e) { return !e->IsActive() || !e->IsInGroup(i); }), v.end());
			}

			m_Entities.erase(std::remove_if(m_Entities.begin(), m_Entities.end(), [](Entity* e) { return !e->IsActive(); }), m_Entities.end());
			*/
		}

	public:
		Entity* AddEntity(const std::string& entityTag = "parent")
		{
			std::string tag = entityTag;
			if (tag == "parent") tag += std::to_string(m_Entities.size());
			std::shared_ptr<Entity> e = std::make_shared<Entity>(*this);
			e->tag = tag;	
			AddToGroup(e.get(), 0);
			m_Entities.push_back(e);
			return e.get();
		}
		void AddToGroup(Entity* parent, Group group)
		{
			m_Groups[group].push_back(parent);
			parent->m_Group = group;
		}
		void SetGroup(Entity* parent, Group group)
		{
			auto& vec = m_Groups[parent->m_Group];
			vec.erase(std::remove(vec.begin(), vec.end(), parent), vec.end());
			AddToGroup(parent, group);
		}
		std::vector<Entity*>& GetGroup(Group group)
		{
			return m_Groups[group];
		}
		void DisposeEntity(Entity* parent)	
		{
			auto it = std::find_if(m_Entities.begin(), m_Entities.end(),
				[&](std::shared_ptr<Entity> e) { return e.get() == parent; });

			if (it != m_Entities.end())
			{
				it->reset();
				m_Entities.erase(it);
			}
		}
		Entity* GetEntityByTag(const std::string& tag)
		{
			auto e = std::find_if(m_Entities.begin(), m_Entities.end(), 
				[&](const std::shared_ptr<Entity> e) { return !e->tag.empty() && e->tag == tag; });
			if (e != m_Entities.end()) return e->get();
			return nullptr;
		}
	};
}

#endif // ECS_H