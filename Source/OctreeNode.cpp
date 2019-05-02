#include "OctreeNode.h"

#include "AABBobj.h"
#include "OBBobj.h"

OctreeNode::OctreeNode(AABoundingBox bounds, std::vector<Collidable*> objs)
{
	m_boundingBox = bounds;
	m_obj = objs;
}

OctreeNode::OctreeNode(AABoundingBox bounds, std::vector<Collidable*> objs, OctreeNode * parent)
{
	m_boundingBox = bounds;
	m_obj = objs;
	m_parent = parent;
	m_depth = parent->m_depth + 1;
}

void OctreeNode::addCollidable(Collidable * obj)
{
	if(obj->checkCollision(m_boundingBox))
	{
		m_obj.push_back(obj);
	}
}

void OctreeNode::checkCollision(GameData* gameData)
{
	if(m_isLeaf)
	{
		for(auto& collidable : m_obj)
		{
			for(auto& other : m_obj)
			{
				if(collidable != other)
				{
					bool collided = false;
					switch(collidable->getCollidableType())
					{
						case CollidableBounds::AABB:
						{
							auto temp = static_cast<AABBobj*>(other);
							collided = collidable->checkCollision(*temp);
							break;
						}
						case CollidableBounds::BS:
							break;
						case CollidableBounds::OBB:
						{
							OBBobj* temp = static_cast<OBBobj*>(other);
							collided = collidable->checkCollision(*temp);
							break;
						}
					}

					if(collided)
					{
						collidable->onHit(other, gameData);
					}
				}
			}
		}
	}
	else
	{
		for(auto& child : m_children)
		{
			if(child)
			{
				child->checkCollision(gameData);
			}
		}
	}
}

void OctreeNode::checkCollision(Collidable * obj, GameData * gameData)
{
	if(m_isLeaf)
	{
		for(auto& other : m_obj)
		{
			if(obj != other)
			{
				bool collided = false;
				switch(other->getCollidableType())
				{
					case CollidableBounds::AABB:
					{
						auto temp = static_cast<AABBobj*>(other);
						collided = obj->checkCollision(*temp);
						break;
					}
					case CollidableBounds::BS:
						break;
					case CollidableBounds::OBB:
					{
						OBBobj* temp = static_cast<OBBobj*>(other);
						collided = obj->checkCollision(*temp);
						break;
					}
				}

				if(collided)
				{
					obj->onHit(other, gameData);
				}

			}
				
		}
	}
	else
	{
		for(auto& child : m_children)
		{
			if(obj->checkCollision(child->m_boundingBox))
			{
				child->checkCollision(obj, gameData);
			}
		}
	}
}

void OctreeNode::buildBox()
{
	glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());

	for(auto& obj : m_obj)
	{
		auto temp = obj->getMin();
		auto temp2 = obj->getMax();
		min = glm::min(min, temp);
		max = glm::max(max, temp2);
	}

	AABoundingBox box = AABoundingBox{min, max, min + (glm::abs(min - max) / 2.0f)};
}

std::unique_ptr<OctreeNode> OctreeNode::createNode(AABoundingBox bounds, std::vector<Collidable*> objs)
{
	if(objs.size() == 0)
		return nullptr;
	else if(objs.size() <= 10)
	{
		auto node = std::make_unique<OctreeNode>(bounds, objs, this);
		node->m_isLeaf = true;
		return node;
	}

	auto node = std::make_unique<OctreeNode>(bounds, objs, this);
	return node;
}

void OctreeNode::buildNode()
{
	if(!m_isLeaf)
	{
		//Pass it to parent if it has one if it doesnt collide
		for(auto& collidable : m_obj)
		{
			if(collidable->getTag() != Tag::STATIC)
			{
				if(!collidable->checkCollision(m_boundingBox))
				{
					if(m_parent)
					{
						m_parent->addCollidable(collidable);
					}
					else
					{
						buildBox();
					}
				}
			}
		}

		std::array<AABoundingBox, 8> octants = getOctants(m_boundingBox);
		std::vector<Collidable*> childObjs[8];

		//Check which octants objects are colliding with
		for(int i = 0; i < 8; i++)
		{
			for(auto& collidable : m_obj)
			{
				if(collidable->checkCollision(octants[i]))
				{
					childObjs[i].push_back(collidable);
				}
			}
		}

		//assign all collidables to octants
		for(int i = 0; i < 8; i++)
		{
			m_children[i] = createNode(octants[i], childObjs[i]);
			
			if(m_children[i])
			{
				m_children[i]->buildNode();
			}
		}
	}
}

const std::array<AABoundingBox, 8>& OctreeNode::getOctants(const AABoundingBox & box)
{
	//Get the dimensions of the box
	glm::vec3 dimensions = glm::abs(box.m_max - box.m_min);

	if(dimensions == glm::vec3(0.0f))
	{
		return std::array<AABoundingBox, 8>();
	}

	glm::vec3 half = dimensions / 2.0f;
	glm::vec3 center = box.m_min + half;

	std::array<AABoundingBox, 8> octants;

	//0
	octants[0] = calcBox(m_boundingBox.m_min, center);

	//1
	octants[1] = calcBox(glm::vec3(center.x, m_boundingBox.m_min.y, m_boundingBox.m_min.z),
							   glm::vec3(m_boundingBox.m_max.x, center.y, center.z));

	//2
	octants[2] = calcBox(glm::vec3(center.x, m_boundingBox.m_min.y, center.z),
							   glm::vec3(m_boundingBox.m_max.x, center.y, m_boundingBox.m_max.z));

	//3

	octants[3] = calcBox(glm::vec3(m_boundingBox.m_min.x, m_boundingBox.m_min.y, center.z),
							   glm::vec3(center.x, center.y, m_boundingBox.m_max.z));

	//4
	octants[4] = calcBox(glm::vec3(m_boundingBox.m_min.x, center.y, m_boundingBox.m_min.z),
							   glm::vec3(center.x, m_boundingBox.m_max.y, center.z));

	//5
	octants[5] = calcBox(glm::vec3(center.x, center.y, m_boundingBox.m_min.z),
							   glm::vec3(m_boundingBox.m_max.x, m_boundingBox.m_max.y, center.z));

	//6
	octants[6] = calcBox(center, m_boundingBox.m_max);

	//7
	octants[7] = calcBox(glm::vec3(m_boundingBox.m_min.x, center.y, center.z),
							   glm::vec3(center.x, m_boundingBox.m_max.y, m_boundingBox.m_max.z));



	return octants;
}

AABoundingBox OctreeNode::calcBox(glm::vec3 min, glm::vec3 max)
{
	return AABoundingBox(min, max, min + (glm::abs(min - max) / 2.0f));
}
