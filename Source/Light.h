#pragma once

#include "GameObject3D.h"

class Light : public GameObject3D
{
public:
	Light() = default;
	virtual ~Light() = default;

private:
	float m_ambientIntensity = 0.5f;
	float m_diffuseIntensity = 0.5f;
};