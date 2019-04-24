#include "OctTree.h"

#include "AABBobj.h"
#include "OBBobj.h"

bool OctTree::s_built = false;
bool OctTree::s_ready = false;
std::deque<Collidable*> OctTree::s_queue = std::deque<Collidable*>();
std::unique_ptr<OctTree> OctTree::s_rootNode = nullptr;
std::vector<Collidable*> OctTree::s_allObjects = std::vector<Collidable*>();

OctTree::OctTree()
{
	m_boundingBox = AABoundingBox(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
}

OctTree::OctTree(const AABoundingBox & bounds)
{
	m_boundingBox = bounds;
}

OctTree::OctTree(const OctTree & other)
{
	m_boundingBox = other.m_boundingBox;
	m_collidable = other.m_collidable;
	m_activeNodes = other.m_activeNodes;
	m_parent = other.m_parent;
	//m_childNodes = other.m_childNodes;


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
			obj->tick(gameData);
		}

		for(int flags = m_activeNodes, i = 0; flags > 0; flags >>= 1, i++)
		{
			if((flags & 1) == 1 && m_childNodes[i]->m_life == 0)
			{
				if(!m_childNodes[i]->m_collidable.empty())
				{
					m_childNodes[i]->m_life = -1;
				}
				else
				{
					m_childNodes[i] = nullptr;
					m_activeNodes ^= (unsigned char)(1 << i);
				}
			}
		}

		for(int flags = m_activeNodes, i = 0; flags > 0; flags >>= 1, i++)
		{
			if((flags & 1) == 1)
			{
				if(!m_childNodes.empty() && m_childNodes[i])
				{
					m_childNodes[i]->tick(gameData);
				}
			}
		}
	}
	else
	{
		if(!s_queue.empty())
		{
			//Process Pending items here
			tick(gameData);
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
	if(m_collidable.size() <= 1)
	{
		if(m_collidable.size() == 1 && m_collidable[0]->checkCollision(m_boundingBox))
		{
			return;
		}
	}

	glm::vec3 dimensions = m_boundingBox.m_max - m_boundingBox.m_min;

	if (dimensions == glm::vec3(0.0f))
	{
		//AABoundingBox box =
		//m_boundingBox = 
		// dimensions
	}

	if (dimensions.x <= MinimumBoxSize && dimensions.y <= MinimumBoxSize && dimensions.z <= MinimumBoxSize)
	{
		return;
	}

	glm::vec3 halfSize = dimensions / 2.0f;
	glm::vec3 center = m_boundingBox.m_min + halfSize;

	std::array<AABoundingBox, 8> octants = getOctants(m_boundingBox);
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
		m_collidable.erase(std::remove_if(m_collidable.begin(), m_collidable.end(), 
						   [&](Collidable* other)
		{
			return obj == other;
		}));
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
			m_collidable.push_back(s_queue.front());
			s_queue.pop_front();
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

const std::array<AABoundingBox, 8>& OctTree::getOctants(const AABoundingBox& box)
{
	glm::vec3 dimensions = box.m_max - box.m_min;

	if(dimensions == glm::vec3(0.0f))
	{
		return std::array<AABoundingBox, 8>();
	}

	glm::vec3 half = dimensions / 2.0f;
	glm::vec3 center = box.m_min + half;

	std::array<AABoundingBox, 8> octants;
	
	//0
	octants[0] = AABoundingBox(m_boundingBox.m_min, center, glm::vec3(0.0f));
		
	//1
	octants[1] = AABoundingBox(glm::vec3(center.x, m_boundingBox.m_min.y, m_boundingBox.m_min.z),
							   glm::vec3(m_boundingBox.m_max.x, center.y, center.z), glm::vec3(0.0f));
	
	//2
	octants[2] = AABoundingBox(glm::vec3(center.x, m_boundingBox.m_min.y, center.z),
							   glm::vec3(m_boundingBox.m_max.x, center.y, m_boundingBox.m_max.z), glm::vec3(0.0f));
		
	//3
	octants[3] = AABoundingBox(glm::vec3(m_boundingBox.m_min.x, m_boundingBox.m_min.y, center.z),
							   glm::vec3(center.x, center.y, m_boundingBox.m_max.z), glm::vec3(0.0f));
		
	//4
	octants[4] = AABoundingBox(glm::vec3(m_boundingBox.m_min.x, center.y, m_boundingBox.m_min.z),
							   glm::vec3(center.x, m_boundingBox.m_max.y, center.z), glm::vec3(0.0f));
		
	//5
	octants[5] = AABoundingBox(glm::vec3(center.x, center.y, m_boundingBox.m_min.z),
							glm::vec3(m_boundingBox.m_max.x, m_boundingBox.m_max.y, center.z), glm::vec3(0.0f));
		
	//6
	octants[6] = AABoundingBox(center, m_boundingBox.m_max, glm::vec3(0.0f));
		
	//7
	octants[7] = AABoundingBox(glm::vec3(m_boundingBox.m_min.x, center.y, center.z),
							   glm::vec3(center.x, m_boundingBox.m_max.y, m_boundingBox.m_max.z), glm::vec3(0.0f));



	return octants;
}

void OctTree::processPendingItems()
{
	if(m_parent == nullptr)
		s_rootNode = std::move(std::make_unique<OctTree>(*this));

	if(!s_built)
	{
		for(auto& obj : s_queue)
		{
			m_collidable.push_back(obj);
		}
		s_queue.clear();

		buildTree();

		s_built = true;
		s_ready = true;
	}
	else
	{
		while(!s_queue.empty())
		{
			for(auto& obj : s_queue)
			{
				insert(obj);
			}
			s_queue.clear();
		}
	}
}

const bool OctTree::insert(Collidable * obj)
{
	if(s_allObjects.empty())
	{
		m_collidable.push_back(obj);
	}

	glm::vec3 dimensions = m_boundingBox.m_max - m_boundingBox.m_min;
	if(dimensions.x <= MinimumBoxSize && dimensions.y <= MinimumBoxSize && dimensions.z <= MinimumBoxSize)
	{
		m_collidable.push_back(obj);
	}

	if(!obj->checkCollision(m_boundingBox))
	{
		if(m_parent != nullptr)
		{
			return m_parent->insert(obj);
		}
		else
		{
			return false;
		}
	}

	AABoundingBox octants[8];
	std::array<AABoundingBox,8> tempOctants = getOctants(m_boundingBox);
	glm::vec3 center = m_boundingBox.m_min + (dimensions / 2.0f);
	
	for(int i = 0; i < tempOctants.size(); i++)
	{
		octants[i] = (m_childNodes[i] != nullptr) ? m_childNodes[i]->m_boundingBox : tempOctants[i];
	}

	if(obj->checkCollision(m_boundingBox))
	{
		bool found = false;

		for(unsigned int i = 0; i < 8; i++)
		{
			if(obj->checkCollision(m_childNodes[i]->m_boundingBox))
			{
				if(m_childNodes[i] != nullptr)
				{
					return m_childNodes[i]->insert(obj);
				}
				else
				{
					//m_childNodes[i] = createNode(octants[i], obj);
					m_activeNodes |= (unsigned char)(1 << i);
				}
			}
			found = true;
		}

		if(!found)
		{
			m_collidable.push_back(obj);
			return true;
		}
	}

	return false;
}

