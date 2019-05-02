#pragma once

#include "glm/glm.hpp"

#include "Helper.h"

#include "GameObject3D.h"

struct Bounds
{
	glm::vec3 m_origin;
};

struct AABoundingBox : public Bounds
{
	AABoundingBox() = default;
	AABoundingBox(glm::vec3 min, glm::vec3 max, glm::vec3 origin) : m_min(min), m_max(max)
	{
		m_origin = origin;
	}

	glm::vec3 m_min; //Min vector of all axes in box
	glm::vec3 m_max; //Max vector of all axes in box
};

struct BoundingSphere : public Bounds
{
	float radius; //radius of sphere
};

struct OBoundingBox : public Bounds
{
	glm::vec3 m_halfLength; //Half length of box
	glm::vec3 m_axes[3]; //Local axes
};

class AABBobj;
class OBBobj;
class BSobj;

enum class CollidableBounds
{
	NONE = 0,
	AABB = 1,
	OBB = 2,
	BS = 3
};

class Collidable : public GameObject3D
{
public:
	Collidable();
	virtual ~Collidable() = default;

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	virtual const bool checkCollision(const glm::vec3& pos) const = 0;
	virtual const bool checkCollision(const AABBobj& other) const = 0;
	virtual const bool checkCollision(const OBBobj& other) const = 0;
	virtual const bool checkCollision(const BSobj& other) const = 0;
 
	virtual const bool checkCollision(const AABoundingBox& other) const = 0;
	virtual const bool checkCollision(const OBoundingBox& other) const = 0;
	virtual const bool checkCollision(const BoundingSphere& other) const = 0;
		    
	const CollidableBounds& getCollidableType() const { return m_collidableType; }
	const bool& hasCollided() const { return m_collided; }
	void setCollided(const bool& collided) { m_collided = collided; }
	virtual void onHit(Collidable* other, GameData* gameData) = 0;

	virtual const glm::vec3 getMin() const = 0;
	virtual const glm::vec3 getMax() const = 0;
protected:
	CollidableBounds m_collidableType = CollidableBounds::NONE;
	bool m_collided = false;
};