#pragma once
#ifndef __MODELIMPORTER_H__
#define __MODELIMPORTER_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <istream>
#include <vector>
#include <string>

using std::vector;

class ModelImporter {
private:
	int num_vertices; // 所有顶点坐标总数
	vector<glm::vec3> vertices; // 所有顶点坐标
	vector<glm::vec2> textures; // 所有纹理坐标
	vector<glm::vec3> vertex_normals; // 所有法线

public:
	ModelImporter() = default;
	void importModel(const char* filePath);
	int getNumVertices();
	vector<glm::vec3> getVertices();
};

#endif __MODELIMPORTER_H__