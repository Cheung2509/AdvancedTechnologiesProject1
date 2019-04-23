#pragma once

#include "Collidable.h"

class OBBobj : public Collidable
{
public:
	OBBobj();
	~OBBobj() = default;

	virtual const bool checkCollision(const glm::vec3& pos) const;
	virtual const bool checkCollision(const AABBobj& other) const;
	virtual const bool checkCollision(const OBBobj& other) const;
	virtual const bool checkCollision(const BSobj& other) const;

	virtual const bool checkCollision(const AABoundingBox& other) const;
	virtual const bool checkCollision(const OBoundingBox& other) const;
	virtual const bool checkCollision(const BoundingSphere& other) const;

	const OBoundingBox& getBox() const { return m_boundingBox; }

	virtual void tick(GameData* gameData) override;
protected:
	OBoundingBox m_boundingBox;
};