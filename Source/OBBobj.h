#pragma once

#include "Collidable.h"
#include "GameObject3D.h"

class OBBobj : public Collidable
{
public:
	OBBobj() = default;
	~OBBobj() = default;

	static bool checkCollision(const OBBobj& obj1, const OBBobj& obj2);
	const OBoundingBox& getBox() const { return m_boundingBox; }

	virtual void tick(GameData* gameData) override;
protected:
	OBoundingBox m_boundingBox;
};