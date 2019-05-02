#include "OctTree.h"

#include "AABBobj.h"
#include "OBBobj.h"

#include<limits>

bool Octree::s_built = false;
bool Octree::s_ready = false;
std::unique_ptr<OctreeNode> Octree::s_rootNode = nullptr;
std::deque<Collidable*> Octree::s_queue = std::deque<Collidable*>();
std::vector<Collidable*> Octree::s_allObjects = std::vector<Collidable*>();

Octree::Octree(std::vector<Collidable*>& collidables)
{
	s_allObjects = collidables;

	if(!s_rootNode)
	{
		glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());

		for(auto& obj : s_allObjects)
		{
			auto temp = obj->getMin();
			auto temp2 = obj->getMax();
			min = glm::min(min, temp);
			max = glm::max(max, temp2);
		}

		AABoundingBox box = AABoundingBox{min, max, min + (glm::abs(min - max) / 2.0f)};
		s_rootNode = std::make_unique<OctreeNode>(box, s_allObjects);
		s_rootNode->buildNode();
	}
}

void Octree::tick(GameData * gameData)
{
	s_rootNode->buildNode();
	for(auto& collidable : s_allObjects)
	{
		if(collidable->getTag() != Tag::STATIC)
		{
			s_rootNode->checkCollision(collidable, gameData);
		}
	}

	/*for(auto& collidable : s_allObjects)
	{
		for(auto& other : s_allObjects)
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
	}*/
}

void Octree::addObj(Collidable * obj)
{
	s_allObjects.push_back(obj);
	s_rootNode->addCollidable(obj);
}
