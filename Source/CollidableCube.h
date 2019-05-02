#pragma once

#include "OBBobj.h"
#include "Model.h"

class CollidableCube : public OBBobj
{
public:
	CollidableCube(std::shared_ptr<Shader> shader);
	~CollidableCube() = default;

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;
private:
	static std::unique_ptr<Model> s_model;
};