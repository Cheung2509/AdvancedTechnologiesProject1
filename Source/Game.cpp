#include "Game.h"

#include "ErrorHandler.h"
#include "Model.h"
#include "Player.h"
#include "Helper.h"

bool Game::init()
{
	//Initialise shader
	std::shared_ptr<Shader> shader = std::make_shared<Shader>
		("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.frag");

	//Initialise game objects
	auto model = std::make_unique<Model>(shader, "Resources/Models/cube.obj");
	model->setScale(glm::vec3(0.25f));

	auto player = std::make_unique<Player>();
	player->init(std::move(model));
	player->setScale(glm::vec3(0.25f));
	m_gameObjects.push_back(std::move(player));

	//Initialise data structures to pass though
	m_gameData = std::make_unique<GameData>();

	m_drawData = std::make_unique<DrawData>();
	m_drawData->m_camera = std::make_shared<Camera>(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f, 
													glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f));

	m_drawData->m_camera->setPos(glm::vec3(0.0f, 5.0f, 5.0f));

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

	//for (auto& obj1 : m_gameObjects)
	//{
	//	for (auto& obj2 : m_gameObjects)
	//	{
	//		if (obj1 != obj2)
	//		{
	//			//Temporary optisation
	//			if (glm::distance(obj1->getPos(), obj2->getPos()) <= 1.5)
	//			{
	//				if (OBBobj::checkCollision(*obj1, *obj2))
	//				{
	//					obj1->setCollided(true);
	//					obj2->setCollided(true);
	//					obj2->pushObject(obj1.get(), m_gameData.get());;
	//				}
	//				else
	//				{
	//					obj1->setCollided(false);
	//					obj2->setCollided(false);
	//				}
	//			}
	//		}
	//	}
	//}
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
