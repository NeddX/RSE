#include "include/BoxCollider2D.h"
#include "../../Core/include/Collision.h"

namespace Advres::RSE
{
	BoxCollider2D::BoxCollider2D(const std::string& tag, Transform transform, Vector2f colliderExtent) : 
		m_Tag(tag), transform(transform), colliderExtent(colliderExtent)
	{
		m_BehaviourObj = nullptr;
		OnCollide = nullptr;
		OnEnter = nullptr;
		OnExit = nullptr;
		m_IsColliding = false;
		trigger = false;
		colour = { 255, 200, 0, 255 };
	}

	BoxCollider2D::~BoxCollider2D()
	{
		RSECore::colliders.erase(std::remove(RSECore::colliders.begin(), RSECore::colliders.end(), this), RSECore::colliders.end());
	}

	void BoxCollider2D::Block()
	{
		// block the owner parent by setting its transform velocity to negative 1
		m_ActorTransform->velocity *= -1;
	}

	void BoxCollider2D::Init()
	{
		m_ActorTransform = this->parent->GetComponent<TransformComponent>();

		RSECore::colliders.push_back(this);
	}

	void BoxCollider2D::Update(float deltaTime)
	{
		colliderRect.x = (int) m_ActorTransform->position.x + (int) transform.position.x;
		colliderRect.y = (int) m_ActorTransform->position.y + (int) transform.position.y;
		colliderRect.w = (int) colliderExtent.x * (int) transform.scale.x * (int) m_ActorTransform->scale.x;
		colliderRect.h = (int) colliderExtent.y * (int) transform.scale.y * (int) m_ActorTransform->scale.y;

		// Ghetto collision detection
		/*for (const auto& c : RSECore::colliders)
		{
			if (c->parent != this->parent)
			{
				if (Collision::AABB(this, c))
				{
					if (!this->m_IsColliding)
					{
						if (this->OnEnter) this->CallBehaviourCallback(this->OnEnter, ComponentCollideResult2D { c->parent, c });
						this->m_IsColliding = true;
					}

					if (!c->m_IsColliding)
					{
						if (c->OnEnter) c->CallBehaviourCallback(c->OnEnter, ComponentCollideResult2D { this->parent, this });
						c->m_IsColliding = true;
					}

					if (!c->trigger) Block();
				}
				else
				{
					// TODO: A collider might be colliding with another collider meaning m_IsColliding is going to be true
					// but this collider is going to think that that collider WAS colliding with IT so it will trigger
					// OnExit and set m_IsColliding of that collider to false even though that collider IS colliding with A
					// DIFFERENT collider!
					if (this->m_IsColliding)
					{
						if (this->OnExit) this->CallBehaviourCallback(this->OnExit, ComponentCollideResult2D { c->parent, c });
						this->m_IsColliding = false;
					}

					if (c->m_IsColliding)
					{
						if (c->OnExit) c->CallBehaviourCallback(c->OnExit, ComponentCollideResult2D { this->parent, this });
						c->m_IsColliding = false;
					}
				}
			}
		}*/
	}

	void BoxCollider2D::Render(float deltaTime)
	{
#ifdef RSE_DEBUG
		RSECore::DrawRect(colliderRect, colour.r, colour.g, colour.b, colour.a);
#endif
	}

	std::string BoxCollider2D::GetTag() const
	{
		return m_Tag;
	}
}
