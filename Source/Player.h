#pragma once

#include "VBO.h"
#include "Keyboard.h"
#include "OBBobj.h"

class Player : public OBBobj
{
public:
	Player() = default;
	Player(std::shared_ptr<VBO> model);
	~Player() = default;

	void init(std::shared_ptr<VBO> model);

	virtual void onHit(Collidable* other, GameData* gameData) override;

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	void handleInput(GameData* keyboard);
private:
	std::shared_ptr<VBO> m_model;
};