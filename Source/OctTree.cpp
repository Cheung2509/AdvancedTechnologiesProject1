#include "OctTree.h"

#include "AABBobj.h"
#include "OBBobj.h"

bool OctTree::s_built = false;
bool OctTree::s_ready = false;
std::queue<Collidable*> OctTree::s_queue = std::queue<Collidable*>();

OctTree::OctTree()
{
	m_boundingBox = AABoundingBox(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
}

OctTree::OctTree(const AABoundingBox & bounds)
{
	m_boundingBox = bounds;
}

void OctTree::tick(GameData * gameData)
{
	if (s_built && s_ready)
	{
		if (m_collidable.empty())
		{
			if (m_childNodes.empty())
			{
				if (m_life == -1)
				{
					m_life = MaxLifeSpan;
				}
				else if (m_life > 0)
				{
					--m_life;
				}
			}
		}
		else
		{
			if (m_life != -1)
			{
				if (m_life <= 64)
					//MaxLifeSpan *= 2;
				m_life = -1;
			}
		}

		std::vector<Collidable*> movedObjects;
		movedObjects.reserve(m_collidable.size());

		for (auto& obj : m_collidable)
		{
			if (!obj->isCollidable())
			{
				movedObjects.push_back(obj);
			}
		}

		for (auto& obj : m_collidable)
		{
			for (auto& other : m_collidable)
			{
				switch (other->getCollidableType())
				{
				case CollidableBounds::AABB:
				{
					auto temp = static_cast<const AABBobj*>(other);
					if (obj->checkCollision(*temp))
					{
						obj->onHit(other);
					}
					break;
				}
				case CollidableBounds::OBB:
				{
					auto temp = static_cast<const OBBobj*>(other);
					if (obj->checkCollision(*temp))
					{
						obj->onHit(other);
					}
					break;
				}
				case CollidableBounds::BS:
				{
					break;
				}
				}
			}
		}
	}
}

OctTree::OctTree(const AABoundingBox & bounds, std::vector<Collidable*>& objs)
{
	m_boundingBox = bounds;
	m_collidable = objs;
}

void OctTree::buildTree()
{
	if (m_collidable.size() <= 1)
		return;

	glm::vec3 dimensions = m_boundingBox.m_max - m_boundingBox.m_min;

	if (dimensions == glm::vec3(0.0f))
	{
		//TODO:
	}

	if (dimensions.x <= MinimumBoxSize && dimensions.y <= MinimumBoxSize && dimensions.z <= MinimumBoxSize)
	{
		return;
	}

	glm::vec3 halfSize = dimensions / 2.0f;
	glm::vec3 center = m_boundingBox.m_min + halfSize;

	AABoundingBox octants[8] = {
		//0
		AABoundingBox(m_boundingBox.m_min, center, glm::vec3(0.0f)),
		
		//1
		AABoundingBox(glm::vec3(center.x, m_boundingBox.m_min.y, m_boundingBox.m_min.z), 
		glm::vec3(m_boundingBox.m_max.x, center.y, center.z), glm::vec3(0.0f)),
		
		//2
		AABoundingBox(glm::vec3(center.x, m_boundingBox.m_min.y, center.z), 
		glm::vec3(m_boundingBox.m_max.x, center.y, m_boundingBox.m_max.z), glm::vec3(0.0f)),

		//3
		AABoundingBox(glm::vec3(m_boundingBox.m_min.x, m_boundingBox.m_min.y, center.z),
		glm::vec3(center.x, center.y, m_boundingBox.m_max.z), glm::vec3(0.0f)),

		//4
		AABoundingBox(glm::vec3(m_boundingBox.m_min.x, center.y, m_boundingBox.m_min.z),
		glm::vec3(center.x, m_boundingBox.m_max.y, center.z), glm::vec3(0.0f)),

		//5
		AABoundingBox(glm::vec3(center.x, center.y, m_boundingBox.m_min.z),
		glm::vec3(m_boundingBox.m_max.x, m_boundingBox.m_max.y, center.z), glm::vec3(0.0f)),

		//6
		AABoundingBox(center, m_boundingBox.m_max, glm::vec3(0.0f)),

		//7
		AABoundingBox(glm::vec3(m_boundingBox.m_min.x, center.y, center.z),
		glm::vec3(center.x, m_boundingBox.m_max.y, m_boundingBox.m_max.z), glm::vec3(0.0f)),
	};

	std::vector<Collidable*> tempVector[8];
	std::vector<Collidable*> delist;

	for (auto& obj : m_collidable)
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			if (obj->checkCollision(octants[i]))
			{
				tempVector[i].push_back(obj);
				delist.push_back(obj);
				break;
			}
		}
	}

	for (auto& obj : delist)
	{
	}

	for (unsigned int i = 0; i <= 8; i++)
	{
		m_childNodes[i] = createNode(octants[i], tempVector[i]);
		m_activeNodes |= (unsigned char)(1 << i);
		m_childNodes[i]->buildTree();
	}

	s_built = true;
	s_ready = true;
}

void OctTree::updateTree()
{
	if (!s_built)
	{
		while (!s_queue.empty())
		{
			m_collidable.emplace_back(s_queue.front());
			s_queue.pop();
			buildTree();
		}
	}
	s_ready = true;
}

std::unique_ptr<OctTree> OctTree::createNode(AABoundingBox box, std::vector<Collidable*>& objs)
{
	if (objs.size() == 0)
		return nullptr;

	return std::make_unique<OctTree>(box, objs);;
}

