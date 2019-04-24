#pragma once

#include <vector>
#include <array>
#include <deque>

#include "Collidable.h"

constexpr int MinimumBoxSize = 1;

class OctTree
{
public:
	OctTree();
	OctTree(const AABoundingBox& bounds, std::vector<Collidable*>& objs);
	OctTree(const AABoundingBox& bounds);
	OctTree(const OctTree& other);

	~OctTree() {}

	void tick(GameData* gameData);

private:
	
	void buildTree();
	void updateTree();
	std::unique_ptr<OctTree> createNode(AABoundingBox box, 
										std::vector<Collidable*>& objs);
	const std::array<AABoundingBox, 8>& getOctants(const AABoundingBox& box);
	void processPendingItems();

	const bool insert(Collidable* obj);

private:
	static std::unique_ptr<OctTree> s_rootNode;

	unsigned char m_activeNodes;
	
	std::vector<Collidable*> m_collidable;
	AABoundingBox m_boundingBox;
	int m_life = -1;

	OctTree* m_parent = nullptr;
	std::array<std::unique_ptr<OctTree>,8> m_childNodes;

	int MaxLifeSpan = 8;

	static std::vector<Collidable*> s_allObjects;
	static std::deque<Collidable*> s_queue;
	static bool s_ready;
	static bool s_built;
};
