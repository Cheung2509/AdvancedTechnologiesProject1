#include "Plane.h"

#include "GameData.h"
#include "Key.h"

Plane::Plane(std::shared_ptr<Shader> shader, const glm::vec2& size)
{
	m_vertices = {
		Vertex{glm::vec3(-size.x, -size.y, 0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(size.x, -size.y, 0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(size.x, size.y, 0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(-size.x, size.y, 0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.0f)}
	};
	
	m_indices =
	{
		0,1,2,2,3,0
	};

	init(shader);
}

Plane::Plane(std::shared_ptr<Shader> shader, const float & x, const float & z)
{
	m_vertices = {
		Vertex{glm::vec3(-x, 0.0f, -z), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(x, 0.0f, -z), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(x, 0.0f, z), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(-x, 0.0f, z), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.0f)}
	};

	m_indices =
	{
		0,1,2,2,3,0
	};

	init(shader);
}

void Plane::tick(GameData * gameData)
{
	VBO::tick(gameData);
}

void Plane::draw(DrawData * drawData)
{
	VBO::draw(drawData);
}
