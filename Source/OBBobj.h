#pragma once

#include "Collidable.h"

class OBBobj : public Collidable
{
public:
	OBBobj() = default;
	~OBBobj() = default;

	virtual bool checkCollision(const glm::vec3& pos) const;
	virtual bool checkCollision(const AABBobj& other) const;
	virtual bool checkCollision(const OBBobj& other) const;
	virtual bool checkCollision(const BSobj& other) const;

	const OBoundingBox& getBox() const { return m_boundingBox; }

	virtual void tick(GameData* gameData) override;
protected:
	OBoundingBox m_boundingBox;
};