#include "VBO.h"

#include "DrawData.h"
#include "GameData.h"

VBO::VBO(std::string vertexShader, std::string fragmentShader)
{
	m_vb = std::make_unique<VertexBuffer>(&m_vertices[0].x, sizeof(glm::vec3) * m_vertices.size());
	m_ib = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

	m_va = std::make_unique<VertexArray>();
	m_va->init();

	m_bufferLayout.push<float>(3);
	m_va->addBuffer(*m_vb, m_bufferLayout);

	m_shader = std::make_shared<Shader>(vertexShader, fragmentShader);
}

VBO::VBO(std::shared_ptr<Shader> shader)
{
	m_vb = std::make_unique<VertexBuffer>(&m_vertices[0].x, sizeof(glm::vec3) * m_vertices.size());
	m_ib = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());
	
	m_va = std::make_unique<VertexArray>();
	m_va->init();
	m_bufferLayout.push<float>(3);
	m_va->addBuffer(*m_vb, m_bufferLayout);

	m_shader = shader;
}

void VBO::init(std::string vertexShader, std::string fragmentShader)
{
	m_vb = std::make_unique<VertexBuffer>(&m_vertices[0].x, sizeof(glm::vec3) * m_vertices.size());
	m_ib = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

	m_va = std::make_unique<VertexArray>();
	m_va->init();
	m_bufferLayout.push<float>(3);
	m_va->addBuffer(*m_vb, m_bufferLayout);

	m_shader = std::make_shared<Shader>(vertexShader, fragmentShader);
}

void VBO::init(std::shared_ptr<Shader> shader)
{
	m_vb = std::make_unique<VertexBuffer>(&m_vertices[0].x, sizeof(glm::vec3) * m_vertices.size());
	m_ib = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

	m_va = std::make_unique<VertexArray>();
	m_va->init();
	m_bufferLayout.push<float>(3);
	m_va->addBuffer(*m_vb, m_bufferLayout);

	m_shader = shader;
}

void VBO::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void VBO::draw(DrawData * drawData)
{
	glm::mat4 mvp = drawData->m_camera->getProjection() * drawData->m_camera->getView() *
		m_worldMatrix;

	m_shader->setUniform4fv("u_MVP", 1, GL_FALSE, mvp);
	m_shader->setUniform4f("u_colour", m_colour.x, m_colour.y, m_colour.z, m_colour.w);

	drawData->m_renderer->draw(*m_va, *m_ib, *m_shader);
}

void VBO::calculateminMax()
{
	//Finding minumum and max for vertices
	for (auto& vertex : m_vertices)
	{
		if (vertex.x < m_min.x)
		{
			m_min.x = vertex.x;
		}
		if (vertex.y < m_min.y)
		{
			m_min.y = vertex.y;
		}
		if (vertex.z < m_min.z)
		{
			m_min.z = vertex.z;
		}

		if (vertex.x > m_max.x)
		{
			m_max.x = vertex.x;
		}
		if (vertex.y > m_max.y)
		{
			m_max.y = vertex.y;
		}
		if (vertex.z > m_max.z)
		{
			m_max.z = vertex.z;
		}
	}
}
