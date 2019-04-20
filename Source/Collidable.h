#pragma once

#include "glm/glm.hpp"

#include "Helper.h"

#include "GameObject3D.h"

struct AABoundingBox
{
	glm::vec3 m_min; //Min vector of all axes in box
	glm::vec3 m_max; //Max vector of all axes in box
};

struct BoundingSphere
{
	glm::vec3 m_origin;
	float radius; //radius of sphere
};

struct OBoundingBox
{
	glm::vec3 m_origin; //Origin of box (Centre)
	glm::vec3 m_halfLength; //Half length of box
	glm::vec3 m_axes[3]; //Local axes
};

class AABBobj;
class OBBobj;
class BSobj;

class Collidable : public GameObject3D
{
public:
	Collidable() = default;
	~Collidable() = default;

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	virtual bool checkCollision(const glm::vec3& pos) const = 0;
	virtual bool checkCollision(const AABBobj& other) const = 0;
	virtual bool checkCollision(const OBBobj& other) const = 0;
	virtual bool checkCollision(const BSobj& other) const = 0;

	const bool& hasCollided() const { return m_collided; }
	void setCollided(const bool& collided) { m_collided = collided; }

protected:
	bool m_collided = false;
};