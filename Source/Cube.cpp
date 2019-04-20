#include "Cube.h"

#include "DrawData.h"
#include "GameData.h"
#include "Key.h"

#include "glm/gtc/matrix_transform.hpp"

Cube::Cube()
{
	m_indices =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3,
	};

	calculateminMax();

	auto shader = std::make_shared<Shader>("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.frag");
	this->init(shader);
}

Cube::Cube(std::string vertexShader, std::string fragmentShader)
{
	m_indices =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3,
	};

	calculateminMax();
	this->init(vertexShader, fragmentShader);
}

Cube::Cube(std::shared_ptr<Shader> shader)
{
	std::vector<glm::vec3> verts =
	{
		// front
		glm::vec3(-1.0, -1.0,  1.0),
		glm::vec3(1.0, -1.0,  1.0),
		glm::vec3(1.0,  1.0,  1.0),
		glm::vec3(-1.0,  1.0,  1.0),
		// back
		glm::vec3(-1.0, -1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),
		glm::vec3(1.0,  1.0, -1.0),
		glm::vec3(-1.0,  1.0, -1.0),
	};

	std::vector<glm::vec3> normals =
	{
		glm::vec3(0,  0, -1),
		glm::vec3(0,  0,  1),
		glm::vec3(-1,  0,  0),
		glm::vec3(1,  0,  0),
		glm::vec3(0, -1,  0),
		glm::vec3(0,  1,  0)
	};

	m_indices =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3,
	};

	calculateminMax();
	this->init(shader);
}

Cube::~Cube()
{
}

void Cube::tick(GameData * gameData)
{
	VBO::tick(gameData);
}

void Cube::draw(DrawData* drawData)
{
	VBO::draw(drawData);
}