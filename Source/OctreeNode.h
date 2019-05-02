#pragma once

#include "Collidable.h"
#include <vector>
#include <array>

class OctreeNode
{
public:
	OctreeNode() = delete;
	OctreeNode(AABoundingBox bounds, std::vector<Collidable*> objs);
	OctreeNode(AABoundingBox bounds, std::vector<Collidable*> objs, OctreeNode* parent);
	~OctreeNode() = default;

	void addCollidable(Collidable* obj);
	void checkCollision(GameData* gameData);
	void checkCollision(Collidable* obj, GameData* gameData);
	void buildNode();
private:
	void buildBox();
	std::unique_ptr<OctreeNode> createNode(AABoundingBox bounds, std::vector<Collidable*> objs);
	
	const std::array<AABoundingBox, 8>& getOctants(const AABoundingBox& box);
	AABoundingBox calcBox(glm::vec3 min, glm::vec3 max);
private:
	int m_depth = 0;

	bool m_isLeaf = false;

	std::array<std::unique_ptr<OctreeNode>, 8> m_children;
	std::vector<Collidable*> m_obj;
	
	AABoundingBox m_boundingBox;
	OctreeNode* m_parent = nullptr;
};