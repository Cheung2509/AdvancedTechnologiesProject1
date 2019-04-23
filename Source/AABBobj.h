#pragma once

#include "GameObject3D.h"
#include "Collidable.h"

class OBBobj;
class BSobj;

class AABBobj : public Collidable
{
public:
	AABBobj();
	~AABBobj() = default;

	const AABoundingBox& getBox() const { return m_boundingBox; }

	virtual const bool checkCollision(const glm::vec3& pos) const;
	virtual const bool checkCollision(const AABBobj& other) const;
	virtual const bool checkCollision(const OBBobj& other) const;
	virtual const bool checkCollision(const BSobj& other) const;

	virtual const bool checkCollision(const AABoundingBox& other) const;
	virtual const bool checkCollision(const OBoundingBox& other) const;
	virtual const bool checkCollision(const BoundingSphere& other) const;

	virtual void onHit(Collidable* other);

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;
protected:
	bool m_drawBox = false;
	AABoundingBox m_boundingBox;
};