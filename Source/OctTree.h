#pragma once

#include <vector>
#include <queue>

#include "Collidable.h"

constexpr int MinimumBoxSize = 1;
constexpr int MaxLifeSpan = 8;

class OctTree
{
public:
	OctTree();
	OctTree(const AABoundingBox& bounds, 
			std::vector<std::shared_ptr<Collidable>>& objs);
	OctTree(const AABoundingBox& bounds);

private:
	
	void buildTree();
	void updateTree();
	std::shared_ptr<OctTree> createNode(AABoundingBox box, 
										std::vector<std::shared_ptr<Collidable>> objs);
private:
	unsigned char m_activeNodes;
	
	std::vector<std::shared_ptr<Collidable>> m_collidable;
	AABoundingBox m_boundingBox;
	int m_life = -1;

	std::shared_ptr<OctTree> m_parent;
	std::shared_ptr<OctTree> m_childNodes[8];
	
	static std::queue<std::shared_ptr<Collidable>> s_queue;
	static bool s_ready;
	static bool s_built;
};