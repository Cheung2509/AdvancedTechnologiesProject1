#include "VBO.h"

#include <limits>

#include "DrawData.h"
#include "GameData.h"

#include "ErrorHandler.h"

VBO::VBO(std::string vertexShader, std::string fragmentShader)
{
	m_vb = std::make_unique<VertexBuffer>(&m_vertices[0], sizeof(glm::vec3) * m_vertices.size());
	m_ib = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

	m_va = std::make_unique<VertexArray>();
	m_va->init();

	m_bufferLayout.push<float>(3);
	m_bufferLayout.push<float>(2);
	m_bufferLayout.push<float>(3);
	m_va->addBuffer(*m_vb, m_bufferLayout);

	m_shader = std::make_shared<Shader>(vertexShader, fragmentShader);
	calculateminMax();
}

VBO::VBO(std::shared_ptr<Shader> shader)
{
	m_vb = std::make_unique<VertexBuffer>(&m_vertices[0], sizeof(glm::vec3) * m_vertices.size());
	m_ib = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

	m_va = std::make_unique<VertexArray>();
	m_va->init();
	m_bufferLayout.push<float>(3);
	m_bufferLayout.push<float>(2);
	m_bufferLayout.push<float>(3);
	m_va->addBuffer(*m_vb, m_bufferLayout);

	m_shader = shader;
	calculateminMax();
}

void VBO::init(std::string vertexShader, std::string fragmentShader)
{
	m_vb = std::make_unique<VertexBuffer>(&m_vertices[0], sizeof(glm::vec3) * m_vertices.size());
	m_ib = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

	m_va = std::make_unique<VertexArray>();
	m_va->init();
	m_bufferLayout.push<float>(3);
	m_bufferLayout.push<float>(2);
	m_bufferLayout.push<float>(3);
	m_va->addBuffer(*m_vb, m_bufferLayout);

	m_shader = std::make_shared<Shader>(vertexShader, fragmentShader);
	calculateminMax();
}

void VBO::init(std::shared_ptr<Shader> shader)
{
	m_vb = std::make_unique<VertexBuffer>(&m_vertices[0], sizeof(Vertex) * m_vertices.size());
	m_ib = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

	m_va = std::make_unique<VertexArray>();
	m_va->init();

	m_va->bind();
	m_vb->bind();
	//Vertex position
	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_pos)));
	//Vertex normals
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_normal)));
	//Vertex uv
	GLCALL(glEnableVertexAttribArray(2));
	GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_uv)));

	m_shader = shader;
	calculateminMax();
}

void VBO::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void VBO::draw(DrawData * drawData)
{
	glm::mat4 mvp = drawData->m_camera->getProjection() * drawData->m_camera->getView() *
		m_worldMatrix;

	glm::vec3 cameraPos = drawData->m_camera->getPos();
	m_shader->setUniform3f("u_light.pos", 0.0f, 0.0f, 0.0f);
	m_shader->setUniform3f("u_light.ambient", 0.2f, 0.2f, 0.2f);
	m_shader->setUniform3f("u_light.diffuse", 0.5f, 0.5f, 0.5f);
	m_shader->setUniform3f("u_light.specular", 1.0f, 1.0f, 1.0f);
	m_shader->setUniform3f("u_viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	m_shader->setUniform3f("u_material.ambient", m_material.m_ambient.x,
						   m_material.m_ambient.y, m_material.m_ambient.z);
	m_shader->setUniform3f("u_material.diffuse", m_material.m_diffuse.x,
						   m_material.m_diffuse.y, m_material.m_diffuse.z);
	m_shader->setUniform3f("u_material.specular", m_material.m_specular.x,
						   m_material.m_specular.y, m_material.m_specular.z);
	m_shader->setUniform1f("u_material.shininess", m_material.m_shininess);

	glm::mat4 view = drawData->m_camera->getView();
	m_shader->setUniform4fv("u_view", 1, GL_FALSE, view);
	m_shader->setUniform4fv("u_model", 1, GL_FALSE, m_worldMatrix);
	m_shader->setUniform4fv("u_MVP", 1, GL_FALSE, mvp);
	m_shader->setUniform4f("u_colour", m_colour.r, m_colour.g, m_colour.b, m_colour.w);

	drawData->m_renderer->draw(*m_va, *m_ib, *m_shader);
}

void VBO::calculateminMax()
{
	float inf = std::numeric_limits<float>::infinity();
	m_min = glm::vec3(inf);
	m_max = glm::vec3(-inf);

	//Finding minumum and max for vertices
	for (auto& vertex : m_vertices)
	{
		m_min = glm::min(vertex.m_pos, m_min);
		m_max = glm::max(vertex.m_pos, m_max);
	}
}