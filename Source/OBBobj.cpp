#include "OBBobj.h"

#include <algorithm>

#include "glm/gtc/quaternion.hpp"

bool OBBobj::checkCollision(const glm::vec3 & pos) const
{
	return false;
}

bool OBBobj::checkCollision(const AABBobj & other) const
{
	return false;
}

bool OBBobj::checkCollision(const OBBobj& other) const
{
	float ra;
	float rb;

	glm::mat3 r;
	glm::mat3 AbsR;

	//Get rotation matrix of the box
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			r[i][j] = glm::dot(m_boundingBox.m_axes[i], other.getBox().m_axes[j]);
		}
	}

	glm::vec3 t = other.getPos() - m_pos;

	t = glm::vec3(glm::dot(t, m_boundingBox.m_axes[0]), 
				  glm::dot(t, m_boundingBox.m_axes[1]), 
				  glm::dot(t, m_boundingBox.m_axes[2]));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			AbsR[i][j] = std::abs(r[i][j]) + std::numeric_limits<float>::epsilon();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = m_boundingBox.m_halfLength[i];
		rb = other.getBox().m_halfLength[0] * AbsR[i][0] +
			other.getBox().m_halfLength[1] * AbsR[i][1] +
			other.getBox().m_halfLength[2] * AbsR[i][2];

		if (std::abs(t[i]) > ra + rb)
		{
			return false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = m_boundingBox.m_halfLength[0] * AbsR[0][i] +
			m_boundingBox.m_halfLength[1] * AbsR[1][i] +
			m_boundingBox.m_halfLength[2] * AbsR[2][i];
		rb = other.getBox().m_halfLength[i];

		if (std::abs(t[0] * r[0][i] + t[1] * r[1][i] + t[2] * r[2][i])
	> ra + rb)
		{
			return false;
		}
	}

	//L = A0 x B0
	ra = m_boundingBox.m_halfLength[1] * AbsR[2][0] +
		 m_boundingBox.m_halfLength[2] * AbsR[1][0];
	rb = other.getBox().m_halfLength[1] * AbsR[0][2] +
		other.getBox().m_halfLength[2] * AbsR[0][1];
	if (std::abs(t[2] * r[1][0] - t[1] * r[2][0]) > ra + rb)
	{
		return false;
	}

	//L = A0 x B1 
	ra = m_boundingBox.m_halfLength[1] * AbsR[2][1] +
		m_boundingBox.m_halfLength[2] * AbsR[1][1];
	rb = other.getBox().m_halfLength[0] * AbsR[0][2] +
		other.getBox().m_halfLength[2] * AbsR[0][0];
	if (std::abs(t[2] * r[1][1] - t[1] * r[2][1]) > ra + rb)
	{
		return false;
	}

	//L = A0 x B2
	ra = m_boundingBox.m_halfLength[1] * AbsR[2][2] +
		m_boundingBox.m_halfLength[2] * AbsR[1][2];
	rb = other.getBox().m_halfLength[0] * AbsR[0][1] +
		other.getBox().m_halfLength[1] * AbsR[0][0];
	if (std::abs(t[2] * r[1][2] - t[1] * r[2][2]) > ra + rb)
	{
		return false;
	}

	//L = A1 x B0
	ra = m_boundingBox.m_halfLength[0] * AbsR[2][0] +
		m_boundingBox.m_halfLength[2] * AbsR[0][0];
	rb = other.getBox().m_halfLength[1] * AbsR[1][2] +
		other.getBox().m_halfLength[2] * AbsR[1][1];
	if (std::abs(t[0] * r[2][0] - t[2] * r[0][0]) > ra + rb)
	{
		return false;
	}

	//L A1 x B1
	ra = m_boundingBox.m_halfLength[0] * AbsR[2][1] +
		m_boundingBox.m_halfLength[2] * AbsR[0][1];
	rb = other.getBox().m_halfLength[0] * AbsR[1][2] +
		other.getBox().m_halfLength[2] * AbsR[1][0];
	if (std::abs(t[0] * r[2][1] - t[2] * r[0][1]) > ra + rb)
	{
		return false;
	}

	//L = A1 x B2
	ra = m_boundingBox.m_halfLength[0] * AbsR[2][2] +
		m_boundingBox.m_halfLength[2] * AbsR[0][2];
	rb = other.getBox().m_halfLength[0] * AbsR[1][1] +
		other.getBox().m_halfLength[1] * AbsR[1][0];
	if (std::abs(t[0] * r[2][2] - t[2] * r[0][2]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B0
	ra = m_boundingBox.m_halfLength[0] * AbsR[1][0] +
		m_boundingBox.m_halfLength[1] * AbsR[0][0];
	rb = other.getBox().m_halfLength[1] * AbsR[2][2] +
		other.getBox().m_halfLength[2] * AbsR[2][1];
	if (std::abs(t[1] * r[0][0] - t[0] * r[1][0]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B1
	ra = m_boundingBox.m_halfLength[0] * AbsR[1][1] +
		m_boundingBox.m_halfLength[1] * AbsR[0][1];
	rb = other.getBox().m_halfLength[0] * AbsR[2][2] +
		other.getBox().m_halfLength[2] * AbsR[2][0];
	if (std::abs(t[1] * r[0][1] - t[0] * r[1][1]) > ra + rb)
	{
		return false;
	}

	//: = A2 x B2
	ra = m_boundingBox.m_halfLength[0] * AbsR[1][2] +
		m_boundingBox.m_halfLength[1] * AbsR[0][2];
	rb = other.getBox().m_halfLength[0] * AbsR[2][1] +
		other.getBox().m_halfLength[1] * AbsR[2][0];
	if (std::abs(t[1] * r[0][2] - t[0] * r[1][2]) > ra + rb)
	{
		return false;
	}

	return true;
}

bool OBBobj::checkCollision(const BSobj & other) const
{
	return false;
}

void OBBobj::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);

	m_boundingBox.m_origin = m_pos;

	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(0.0f, 0.0f, 1.0f);

	m_boundingBox.m_axes[0] = glm::normalize(forward * m_rotation);
	m_boundingBox.m_axes[1] = glm::normalize(up * m_rotation);
	m_boundingBox.m_axes[2] = glm::normalize(right * m_rotation);
}
