#include "CollidableCube.h"

std::unique_ptr<Model> CollidableCube::s_model = nullptr;

CollidableCube::CollidableCube(std::shared_ptr<Shader> shader)
{
	if(!s_model)
	{
		s_model = std::make_unique<Model>(shader, "Models/Cube_Model.obj");
	}

	m_boundingBox.m_origin = m_pos;
	m_boundingBox.m_halfLength = glm::vec3(std::abs(s_model->getMin().x - s_model->getMax().x) / 2,
										   std::abs(s_model->getMin().y - s_model->getMax().y) / 2,
										   std::abs(s_model->getMin().z - s_model->getMax().z) / 2) * m_scale;

	m_type = Tag::STATIC;

	m_boundingBox.m_axes[0] = glm::vec3(1.0f, 0.0f, 0.0f);
	m_boundingBox.m_axes[1] = glm::vec3(0.0f, 1.0f, 0.0f);
	m_boundingBox.m_axes[1] = glm::vec3(0.0f, 0.0f, 1.0f);
}

void CollidableCube::tick(GameData * gameData)
{
	if (m_collided)
	{
		m_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}

	OBBobj::tick(gameData);

	m_boundingBox.m_origin = m_pos;
}

void CollidableCube::draw(DrawData * drawData)
{
	//Draw model
	s_model->setWorld(this->m_worldMatrix);
	s_model->setColour(m_colour);
	s_model->draw(drawData);
}