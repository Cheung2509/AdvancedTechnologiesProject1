#pragma once

#include "VBO.h"

class Model : public VBO
{
public:
	Model() = delete;
	Model(std::shared_ptr<Shader> shader, const std::string& filePath);
	~Model() {};

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;
private:
	void loadObjFile(const std::string& filePath, std::vector<glm::vec3>& vertices,
					 std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals);
	void indexVBO(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs,
				  std::vector<glm::vec3>& normals);
};