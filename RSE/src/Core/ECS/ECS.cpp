#include "include/ECS.h"

namespace RSE
{
	void RSE::Entity::AddToGroup(Group group)
	{
		m_GroupBitSet[group] = true;
		m_Manager.AddToGroup(this, group);
	}
}
