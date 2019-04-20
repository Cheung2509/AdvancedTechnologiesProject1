#pragma once

#include "GameObject3D.h"
#include "Collidable.h"

class AABBobj : public Collidable
{
public:
	AABBobj() = default;
	~AABBobj() = default;

	const AABoundingBox& getBox() const { return m_boundingBox; }

	static bool checkCollision(AABBobj& obj, const glm::vec3& pos);
	static bool checkCollision(AABBobj& obj1, AABBobj& obj2);

	virtual void tick(GameData* gameData) override;
protected:
	bool m_drawBox = false;
	AABoundingBox m_boundingBox;
};