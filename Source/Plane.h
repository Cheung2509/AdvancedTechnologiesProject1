#pragma once

#include "VBO.h"

class Plane : public VBO
{
public:
	Plane(std::shared_ptr<Shader> shader = nullptr, const glm::vec2& size = glm::vec2(10.0f));
	Plane(std::shared_ptr<Shader> shader = nullptr, const float& x = 10.0f, const float& z = 10.0f);
	~Plane() = default;

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;
private:
};