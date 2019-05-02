#pragma once

#include <vector>
#include <array>
#include <deque>

#include "Collidable.h"
#include "OctreeNode.h"

class Octree
{
public:
	Octree() = default;
	Octree(std::vector<Collidable*>& collidables);

	~Octree() {}

	void tick(GameData* gameData);
	void addObj(Collidable* obj);

private:
	static std::unique_ptr<OctreeNode> s_rootNode;

	static std::vector<Collidable*> s_allObjects;
	static std::deque<Collidable*> s_queue;
	static bool s_ready;
	static bool s_built;
};
