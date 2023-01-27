#include "include/ECS.h"

namespace Advres::RSE
{
	void Entity::SetGroup(Group group)
	{
		m_Manager.SetGroup(this, group);
	}
}
