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

	virtual bool checkCollision(const glm::vec3& pos) const;
	virtual bool checkCollision(const AABBobj& other) const;
	virtual bool checkCollision(const OBBobj& other) const;
	virtual bool checkCollision(const BSobj& other) const;

	virtual bool checkCollision(const AABoundingBox& other) const;
	virtual bool checkCollision(const OBoundingBox& other) const;
	virtual bool checkCollision(const BoundingSphere& other) const;

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;
protected:
	bool m_drawBox = false;
	AABoundingBox m_boundingBox;
};