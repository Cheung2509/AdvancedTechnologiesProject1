#pragma once

#include <vector>
#include <memory>

class Collidable;

class CollisionManager
{
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	void checkCollision();

	void addObject(std::shared_ptr<Collidable> obj);
protected:

private:
	std::vector<std::shared_ptr<Collidable>> m_objs;
};