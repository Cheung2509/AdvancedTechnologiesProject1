#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <memory>

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

struct GameData;
struct DrawData;
class Renderer;

enum class Tag
{
	NONE = 0,
	PLAYER = 1, 
	ENEMY = 2,
	PLANE = 3,
	STATIC = 4
};

class GameObject3D
{
public:
	GameObject3D();
	virtual ~GameObject3D();

	virtual void tick(GameData* gameData);
	virtual void draw(DrawData* drawData) = 0;

	virtual void pushObject(GameObject3D* obj1, GameData* gameData);
	//Getters
	const glm::vec3& getPos() const { return m_pos; }
	const glm::vec3& getScale() const { return m_scale; }
	const glm::vec4& getColour() const { return m_colour; }
	const glm::quat& getRotation() const { return m_rotation; }
	const bool& isCollidable() const { return m_collidable; }
	const Tag& getTag() const { return m_type; }

	//Setters
	void setPos(const glm::vec3& newPos) { m_pos = newPos; }
	void setScale(const glm::vec3& newScale) { m_scale = newScale; }
	void setColour(const glm::vec4& colour) { m_colour = colour; }
	void rotate(const float& angle, const glm::vec3& axis) { m_rotation = glm::rotate(m_rotation, glm::radians(angle), axis); }
protected:
	glm::vec3 m_pos;
	glm::vec3 m_scale;

	glm::quat m_rotation;

	glm::vec4 m_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	glm::mat4 m_worldMatrix;
	glm::mat4 m_rotationMatrix;

	bool m_collidable = false;
	bool m_physics = false;

	Tag m_type = Tag::NONE;
};