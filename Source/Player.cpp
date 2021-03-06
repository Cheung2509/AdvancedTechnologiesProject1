#include "Player.h"

#include "GameData.h"
#include "Key.h"

Player::Player(std::shared_ptr<VBO> model)
{
	m_type = Tag::PLAYER;
}

void Player::init(std::shared_ptr<VBO> model)
{
	m_model = model;
	m_boundingBox.m_origin = m_pos;
	m_boundingBox.m_halfLength = glm::vec3(glm::abs(m_model->getMax() - m_model->getMin()) / 2.0f) * m_scale;

	m_boundingBox.m_axes[0] = glm::vec3(1.0f, 0.0f, 0.0f);
	m_boundingBox.m_axes[1] = glm::vec3(0.0f, 1.0f, 0.0f);
	m_boundingBox.m_axes[2] = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Player::onHit(Collidable* other, GameData* gameData)
{
	pushObject(other, gameData);
}

void Player::tick(GameData * gameData)
{
	handleInput(gameData);

	if (m_collided)
	{
		m_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	OBBobj::tick(gameData);
}

void Player::draw(DrawData * drawData)
{
	//Draw model
	m_model->setWorld(this->m_worldMatrix);
	m_model->setColour(m_colour);
	m_model->draw(drawData);
}

void Player::handleInput(GameData* gameData)
{
	if (gameData->m_keyboard.keyIsPressed(Key::KEY_A))
	{
		m_pos -= glm::vec3(1.0f, 0.0f, 0.0f) * gameData->m_deltaTime;
	}

	if (gameData->m_keyboard.keyIsPressed(Key::KEY_D))
	{
		m_pos += glm::vec3(1.0f, 0.0f, 0.0f) * gameData->m_deltaTime;
	}

	if (gameData->m_keyboard.keyIsPressed(Key::KEY_W))
	{
		m_pos -= glm::vec3(0.0f, 0.0f, 1.0f) * gameData->m_deltaTime;
	}

	if (gameData->m_keyboard.keyIsPressed(Key::KEY_S))
	{
		m_pos += glm::vec3(0.0f, 0.0f, 1.0f) * gameData->m_deltaTime;
	}

	if (gameData->m_keyboard.keyIsPressed(Key::KEY_Q))
	{
		m_rotation = glm::rotate(m_rotation, 1.0f * gameData->m_deltaTime,
								 glm::vec3(0.0f, 1.0f, 0.0f));;
	}
	else if (gameData->m_keyboard.keyIsPressed(Key::KEY_E))
	{
		m_rotation = glm::rotate(m_rotation, 1.0f * gameData->m_deltaTime,
								 glm::vec3(0.0f, -1.0f, 0.0f));;
	}
}