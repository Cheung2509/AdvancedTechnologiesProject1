#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight() = default;
	~PointLight() = default;
private:
	struct
	{
		float m_constant;
		float linear;
		float exp;
	} m_attenuation;
};