#pragma once
#include "glm/glm.hpp"

struct Light
{
	glm::vec3 m_colour;
	float m_ambient;
	float m_diffuse;
};

struct PointLight : public Light
{
	glm::vec3 m_position;

	struct
	{
		float m_constant;
		float m_linear;
		float m_exp;
	}Attenuation;
};