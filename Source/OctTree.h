#pragma once

#include <vector>
#include <array>
#include <queue>

#include "Collidable.h"

constexpr int MinimumBoxSize = 1;
constexpr int MaxLifeSpan = 8;

class OctTree
{
public:
	OctTree();
	OctTree(const AABoundingBox& bounds, std::vector<Collidable*>& objs);
	OctTree(const AABoundingBox& bounds);

	void tick(GameData* gameData);

private:
	
	void buildTree();
	void updateTree();
	std::unique_ptr<OctTree> createNode(AABoundingBox box, 
										std::vector<Collidable*>& objs);
private:
	unsigned char m_activeNodes;
	
	std::vector<Collidable*> m_collidable;
	AABoundingBox m_boundingBox;
	int m_life = -1;

	std::shared_ptr<OctTree> m_parent;
	std::array<std::unique_ptr<OctTree>,8> m_childNodes;
	
	static std::queue<Collidable*> s_queue;
	static bool s_ready;
	static bool s_built;
};