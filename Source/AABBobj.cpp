#include "AABBobj.h"

#include "OBBobj.h"

AABBobj::AABBobj()
{
	m_collidableType = CollidableBounds::AABB;
	m_collidable = true;
}

const bool AABBobj::checkCollision(const glm::vec3& pos) const
{
	return m_boundingBox.m_min.x + m_pos.x < pos.x &&
		m_boundingBox.m_max.x + m_pos.x > pos.x &&
		m_boundingBox.m_min.y + m_pos.y < pos.y &&
		m_boundingBox.m_max.y + m_pos.y > pos.y &&
		m_boundingBox.m_min.z + m_pos.z < pos.z &&
		m_boundingBox.m_max.z + m_pos.z > pos.z;
}

const bool AABBobj::checkCollision(const AABBobj& other) const
{
	AABoundingBox otherBox = other.getBox();
	return m_boundingBox.m_max.x + m_pos.x > otherBox.m_min.x + otherBox.m_origin.x &&
		m_boundingBox.m_min.x + m_pos.x < otherBox.m_max.x + otherBox.m_origin.x &&
		m_boundingBox.m_max.y + m_pos.y > otherBox.m_min.y + otherBox.m_origin.y &&
		m_boundingBox.m_min.y + m_pos.y < otherBox.m_max.y + otherBox.m_origin.y &&
		m_boundingBox.m_max.z + m_pos.z > otherBox.m_min.z + otherBox.m_origin.z &&
		m_boundingBox.m_min.z + m_pos.z < otherBox.m_max.z + otherBox.m_origin.z;
}

const bool AABBobj::checkCollision(const OBBobj & other) const
{
	//Calculation the half lengths and axis for this AABB
	glm::vec3 halfLength = glm::vec3(std::abs(m_boundingBox.m_min.x - m_boundingBox.m_max.x) / 2,
									 std::abs(m_boundingBox.m_min.y - m_boundingBox.m_max.y) / 2,
									 std::abs(m_boundingBox.m_min.z - m_boundingBox.m_max.z) / 2) * m_scale;

	glm::vec3 axes[3] = {glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f) * m_rotation),
		glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) * m_rotation),
		glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f) * m_rotation),};

	float ra;
	float rb;

	glm::mat3 r;
	glm::mat3 AbsR;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			r[i][j] = glm::dot(axes[i], other.getBox().m_axes[j]);
		}
	}

	glm::vec3 t = other.getPos() - m_pos;
	t = glm::vec3(glm::dot(t, axes[0]),
				  glm::dot(t, axes[1]),
				  glm::dot(t, axes[2]));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			AbsR[i][j] = glm::abs(r[i][j]) + std::numeric_limits<float>::epsilon();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = halfLength[i];
		rb = other.getBox().m_halfLength[0] * AbsR[i][0] +
			other.getBox().m_halfLength[1] * AbsR[i][1] +
			other.getBox().m_halfLength[2] * AbsR[i][2];

		if (glm::abs(t[i]) > ra + rb)
		{
			return false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = halfLength[i];
		rb = other.getBox().m_halfLength[0] * AbsR[0][i] +
			other.getBox().m_halfLength[1] * AbsR[1][i] +
			other.getBox().m_halfLength[2] * AbsR[2][i];

		if (glm::abs(t[0] * r[0][i] + t[1] * r[1][i] + t[2] * r[2][i]) > ra + rb)
		{
			return false;
		}
	}

	//L = A0 x B0
	ra = halfLength[1] * AbsR[2][0] + halfLength[2] * AbsR[1][0];
	rb = other.getBox().m_halfLength[1] * AbsR[0][2] +
		other.getBox().m_halfLength[2] * AbsR[0][1];
	if (std::abs(t[2] * r[1][0] - t[1] * r[2][0]) > ra + rb)
	{
		return false;
	}

	//L = A0 x B1
	ra = halfLength[1] * AbsR[2][1] + halfLength[2] * AbsR[1][1];
	rb = other.getBox().m_halfLength[0] * AbsR[0][2] +
		other.getBox().m_halfLength[2] * AbsR[0][0];
	if (std::abs(t[2] * r[1][1] - t[1] * r[2][1]) > ra + rb)
	{
		return false;
	}

	//L = A0 x B2
	ra = halfLength[1] * AbsR[2][2] + halfLength[2] * AbsR[1][2];
	rb = other.getBox().m_halfLength[0] * AbsR[0][1] +
		other.getBox().m_halfLength[1] * AbsR[0][0];
	if (std::abs(t[2] * r[1][2] - t[1] * r[2][2]) > ra + rb)
	{
		return false;
	}

	//L = A1 x B0
	ra = halfLength[0] * AbsR[2][0] + halfLength[2] * AbsR[0][0];
	rb = other.getBox().m_halfLength[1] * AbsR[1][2] +
		other.getBox().m_halfLength[2] * AbsR[1][1];
	if (std::abs(t[0] * r[2][0] - t[2] * r[0][0]) > ra + rb)
	{
		return false;
	}

	//L A1 x B1
	ra = halfLength[0] * AbsR[2][1] + halfLength[2] * AbsR[0][1];
	rb = other.getBox().m_halfLength[0] * AbsR[1][2] +
		other.getBox().m_halfLength[2] * AbsR[1][0];
	if (std::abs(t[0] * r[2][1] - t[2] * r[0][1]) > ra + rb)
	{
		return false;
	}

	//L = A1 x B2
	ra = halfLength[0] * AbsR[2][2] + halfLength[2] * AbsR[0][2];
	rb = other.getBox().m_halfLength[0] * AbsR[1][1] +
		other.getBox().m_halfLength[1] * AbsR[1][0];
	if (std::abs(t[0] * r[2][2] - t[2] * r[0][2]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B0
	ra = halfLength[0] * AbsR[1][0] + halfLength[1] * AbsR[0][0];
	rb = other.getBox().m_halfLength[1] * AbsR[2][2] +
		other.getBox().m_halfLength[2] * AbsR[2][1];
	if (std::abs(t[1] * r[0][0] - t[0] * r[1][0]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B1
	ra = halfLength[0] * AbsR[1][1] + halfLength[1] * AbsR[0][1];
	rb = other.getBox().m_halfLength[0] * AbsR[2][2] +
		other.getBox().m_halfLength[2] * AbsR[2][0];
	if (std::abs(t[1] * r[0][1] - t[0] * r[1][1]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B2
	ra = halfLength[0] * AbsR[1][2] + halfLength[1] * AbsR[0][2];
	rb = other.getBox().m_halfLength[0] * AbsR[2][1] +
		other.getBox().m_halfLength[1] * AbsR[2][0];
	if (std::abs(t[1] * r[0][2] - t[0] * r[1][2]) > ra + rb)
	{
		return false;
	}

	return true;
}

const bool AABBobj::checkCollision(const BSobj & other) const
{
	return false;
}

const bool AABBobj::checkCollision(const AABoundingBox & other) const
{
	return m_boundingBox.m_max.x + m_pos.x > other.m_min.x + other.m_origin.x &&
		m_boundingBox.m_min.x + m_pos.x < other.m_max.x + other.m_origin.x &&
		m_boundingBox.m_max.y + m_pos.y > other.m_min.y + other.m_origin.y &&
		m_boundingBox.m_min.y + m_pos.y < other.m_max.y + other.m_origin.y &&
		m_boundingBox.m_max.z + m_pos.z > other.m_min.z + other.m_origin.z &&
		m_boundingBox.m_min.z + m_pos.z < other.m_max.z + other.m_origin.z;
}

const bool AABBobj::checkCollision(const OBoundingBox & other) const
{
	//Calculation the half lengths and axis for this AABB
	glm::vec3 halfLength = glm::vec3(std::abs(m_boundingBox.m_min.x - m_boundingBox.m_max.x) / 2,
									 std::abs(m_boundingBox.m_min.y - m_boundingBox.m_max.y) / 2,
									 std::abs(m_boundingBox.m_min.z - m_boundingBox.m_max.z) / 2) * m_scale;

	glm::vec3 axes[3] = {glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f) * m_rotation),
		glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) * m_rotation),
		glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f) * m_rotation),};

	float ra;
	float rb;

	glm::mat3 r;
	glm::mat3 AbsR;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			r[i][j] = glm::dot(axes[i], other.m_axes[j]);
		}
	}

	glm::vec3 t = other.m_origin - m_pos;
	t = glm::vec3(glm::dot(t, axes[0]),
				  glm::dot(t, axes[1]),
				  glm::dot(t, axes[2]));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			AbsR[i][j] = glm::abs(r[i][j]) + std::numeric_limits<float>::epsilon();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = halfLength[i];
		rb = other.m_halfLength[0] * AbsR[i][0] +
			other.m_halfLength[1] * AbsR[i][1] +
			other.m_halfLength[2] * AbsR[i][2];

		if (glm::abs(t[i]) > ra + rb)
		{
			return false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ra = halfLength[i];
		rb = other.m_halfLength[0] * AbsR[0][i] +
			other.m_halfLength[1] * AbsR[1][i] +
			other.m_halfLength[2] * AbsR[2][i];

		if (glm::abs(t[0] * r[0][i] + t[1] * r[1][i] + t[2] * r[2][i]) > ra + rb)
		{
			return false;
		}
	}

	//L = A0 x B0
	ra = halfLength[1] * AbsR[2][0] + halfLength[2] * AbsR[1][0];
	rb = other.m_halfLength[1] * AbsR[0][2] +
		other.m_halfLength[2] * AbsR[0][1];
	if (std::abs(t[2] * r[1][0] - t[1] * r[2][0]) > ra + rb)
	{
		return false;
	}

	//L = A0 x B1
	ra = halfLength[1] * AbsR[2][1] + halfLength[2] * AbsR[1][1];
	rb = other.m_halfLength[0] * AbsR[0][2] +
		other.m_halfLength[2] * AbsR[0][0];
	if (std::abs(t[2] * r[1][1] - t[1] * r[2][1]) > ra + rb)
	{
		return false;
	}

	//L = A0 x B2
	ra = halfLength[1] * AbsR[2][2] + halfLength[2] * AbsR[1][2];
	rb = other.m_halfLength[0] * AbsR[0][1] +
		other.m_halfLength[1] * AbsR[0][0];
	if (std::abs(t[2] * r[1][2] - t[1] * r[2][2]) > ra + rb)
	{
		return false;
	}

	//L = A1 x B0
	ra = halfLength[0] * AbsR[2][0] + halfLength[2] * AbsR[0][0];
	rb = other.m_halfLength[1] * AbsR[1][2] +
		other.m_halfLength[2] * AbsR[1][1];
	if (std::abs(t[0] * r[2][0] - t[2] * r[0][0]) > ra + rb)
	{
		return false;
	}

	//L A1 x B1
	ra = halfLength[0] * AbsR[2][1] + halfLength[2] * AbsR[0][1];
	rb = other.m_halfLength[0] * AbsR[1][2] +
		other.m_halfLength[2] * AbsR[1][0];
	if (std::abs(t[0] * r[2][1] - t[2] * r[0][1]) > ra + rb)
	{
		return false;
	}

	//L = A1 x B2
	ra = halfLength[0] * AbsR[2][2] + halfLength[2] * AbsR[0][2];
	rb = other.m_halfLength[0] * AbsR[1][1] +
		other.m_halfLength[1] * AbsR[1][0];
	if (std::abs(t[0] * r[2][2] - t[2] * r[0][2]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B0
	ra = halfLength[0] * AbsR[1][0] + halfLength[1] * AbsR[0][0];
	rb = other.m_halfLength[1] * AbsR[2][2] +
		other.m_halfLength[2] * AbsR[2][1];
	if (std::abs(t[1] * r[0][0] - t[0] * r[1][0]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B1
	ra = halfLength[0] * AbsR[1][1] + halfLength[1] * AbsR[0][1];
	rb = other.m_halfLength[0] * AbsR[2][2] +
		other.m_halfLength[2] * AbsR[2][0];
	if (std::abs(t[1] * r[0][1] - t[0] * r[1][1]) > ra + rb)
	{
		return false;
	}

	//L = A2 x B2
	ra = halfLength[0] * AbsR[1][2] + halfLength[1] * AbsR[0][2];
	rb = other.m_halfLength[0] * AbsR[2][1] +
		other.m_halfLength[1] * AbsR[2][0];
	if (std::abs(t[1] * r[0][2] - t[0] * r[1][2]) > ra + rb)
	{
		return false;
	}

	return true;
}

const bool AABBobj::checkCollision(const BoundingSphere & other) const
{
	return false;
}

void AABBobj::onHit(Collidable* other)
{
}

void AABBobj::tick(GameData * gameData)
{
	Collidable::tick(gameData);
	m_boundingBox.m_origin = m_pos;
}

void AABBobj::draw(DrawData * drawData)
{
	//if (m_drawBox)
	//{
	//	//TODO: DrawBox
	//}
}