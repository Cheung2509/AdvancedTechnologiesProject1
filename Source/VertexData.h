#pragma once

#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 m_pos;
	glm::vec2 m_uv;
	glm::vec3 m_normal;

	bool operator < (const Vertex that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(Vertex)) > 0;
	}
};

struct Material
{
	glm::vec3 m_ambient = glm::vec3(1.0f,0.5f,0.31f);
	glm::vec3 m_diffuse = glm::vec3(1.0f,0.5f,0.31f);
	glm::vec3 m_specular = glm::vec3(0.5f,0.5f,0.5f);
	float m_shininess = 32.0f;
};