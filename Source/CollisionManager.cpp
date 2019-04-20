#include "CollisionManager.h"

#include "Collidable.h"

void CollisionManager::checkCollision()
{
	for (auto& collidable : m_objs)
	{
		for (auto& other : m_objs)
		{
			if (collidable != other)
			{
				//if (collidable->checkCollision(other))
				//{
				//	//collidable->onHit(other);
				//}
			}
		}
	}
}

void CollisionManager::addObject(std::shared_ptr<Collidable> obj)
{
	m_objs.push_back(obj);
}
