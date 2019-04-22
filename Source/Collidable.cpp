#include "Collidable.h"

Collidable::Collidable()
{
	m_collidable = true;
}

void Collidable::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void Collidable::draw(DrawData * drawData)
{
	
}
