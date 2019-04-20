#include "Model.h"

#include <iostream>
#include <map>

Model::Model(std::shared_ptr<Shader> shader, const std::string & filePath)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadObjFile(filePath, vertices, uvs, normals);
	indexVBO(vertices, uvs, normals);

	this->init(shader);
}

void Model::tick(GameData * gameData)
{
	VBO::tick(gameData);
}

void Model::draw(DrawData * drawData)
{
	VBO::draw(drawData);
}

void Model::loadObjFile(const std::string & filePath, std::vector<glm::vec3>& vertices, 
						std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals)
{
	FILE* file = new FILE();
	fopen_s(&file, filePath.c_str(), "r");

	if (!file)
	{
		std::cout << "Could not opent obj file: " << filePath << std::endl;
		return;
	}

	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;

	std::vector<glm::vec3> temp_v;
	std::vector<glm::vec2> temp_uv;
	std::vector<glm::vec3> temp_n;

	while (1)
	{
		char lineHeader[128];

		int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF)
		{
			break;
		}

		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex = glm::vec3();
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_v.emplace_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv = glm::vec2();
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			temp_uv.emplace_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal = glm::vec3();
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_n.emplace_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1;
			std::string vertex2;
			std::string vertex3;
			unsigned int vertexIndex[3];
			unsigned int uvIndex[3];
			unsigned int normalIndex[3];

			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
								   &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								   &vertexIndex[1], &uvIndex[1], &normalIndex[1],
								   &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9)
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec2 uv(0.0f);

		glm::vec3 vertex = temp_v[vertexIndex - 1];
		if (!temp_uv.empty())
		{
			uv = temp_uv[uvIndex - 1];
		}
		glm::vec3 normal = temp_n[normalIndex - 1];

		vertices.emplace_back(vertex);
		uvs.emplace_back(uv);
		normals.emplace_back(normal);
	}

	fclose(file);
}

void Model::indexVBO(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs,
					 std::vector<glm::vec3>& normals)
{
	std::map<Vertex, unsigned short> map;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		Vertex data = {vertices[i], normals[i], uvs[i]};

		unsigned short index;
		bool found = false;

		if (map.find(data) == map.end())
		{
			found = false;
		}
		else
		{
			found = true;
			index = map[data];
		}

		if (found)
		{
			m_indices.push_back(index);
		}
		else
		{
			m_vertices.push_back(data);
			unsigned short newIndex = m_vertices.size() - 1;
			m_indices.push_back(newIndex);
			map[data] = newIndex;
		}
		
	}
}
