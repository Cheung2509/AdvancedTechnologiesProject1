#include "Game.h"

#include "ErrorHandler.h"
#include "Model.h"
#include "Player.h"
#include "CollidableCube.h"
#include "Plane.h"
#include "Helper.h"

bool Game::init()
{
	//Initialise shader
	std::shared_ptr<Shader> shader = std::make_shared<Shader>
		("Shaders/VertexShader.glsl", "Shaders/FragmentShader.frag");

	//Initialise game objects
	auto model = std::make_shared<Model>(shader, "Models/Cube_Model.obj");
	model->setScale(glm::vec3(0.25f));

	auto sphere = std::make_unique<Model>(shader, "Models/Sphere.obj");
	sphere->setScale(glm::vec3(0.5f));
	sphere->setColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	sphere->setPos(glm::vec3(2.0f, -1.0f, 2.0f));
	m_gameObjects.emplace_back(std::move(sphere));

	auto plane = std::make_unique<Plane>(shader, glm::vec3(10.0f));
	plane->setPos(glm::vec3(0.0f, -10.0f, 0.0f));
	plane->rotate(90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
	plane->setColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_gameObjects.push_back(std::move(plane));

	auto player = std::make_unique<Player>();
	player->init(model);
	m_gameObjects.push_back(std::move(player));

	for(int x = -2; x < 2; x++)
	{
		for(int z = -2; z < 2; z++)
		{
			if(x != 0 && z != 0)
			{
				auto cube = std::make_unique<CollidableCube>(shader);
				cube->setPos(glm::vec3(x * 2.0f, 0.0f, z * 2.0f));
				m_gameObjects.push_back(std::move(cube));
			}
		}
	}

	auto lightSource = std::make_unique<Model>(shader, "Models/Cube_Model.obj");
	lightSource->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
	lightSource->setColour(glm::vec4(1.0f,1.0f,1.0f,1.0f));
	lightSource->setScale(glm::vec3(0.25));
	m_gameObjects.push_back(std::move(lightSource));

	//Initialise data structures to pass though
	m_gameData = std::make_unique<GameData>();

	m_drawData = std::make_unique<DrawData>();
	m_drawData->m_camera = std::make_shared<Camera>(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f,
													glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	m_drawData->m_camera->setPos(glm::vec3(0.0f, 5.0f, 5.0f));

	std::vector<Collidable*> collidables;
	for(auto& obj : m_gameObjects)
	{
		if(obj->isCollidable())
		{
			auto tempPtr = static_cast<Collidable*>(obj.get());
			collidables.push_back(tempPtr);
		}
	}

	m_octree = std::make_unique<Octree>(collidables);

	return true;
}

void Game::tick(const float& deltaTime, const Keyboard& keyboard, const Mouse& mouse)
{
	m_gameData->m_deltaTime = deltaTime;
	m_gameData->m_keyboard = keyboard;
	m_gameData->m_mouse = mouse;

	if (keyboard.keyIsPressed(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	m_drawData->m_camera->tick(m_gameData.get());

	for (auto& object : m_gameObjects)
	{
		object->tick(m_gameData.get());
	}

	m_octree->tick(m_gameData.get());
}

void Game::draw(std::shared_ptr<Renderer> renderer)
{
	renderer->clear();

	m_drawData->m_renderer = renderer;

	for (auto& object : m_gameObjects)
	{
		object->draw(m_drawData.get());
	}
}