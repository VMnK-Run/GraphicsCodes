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
	int num_vertices; // ���ж�����������
	vector<glm::vec3> vertices; // ���ж�������
	vector<glm::vec2> textures; // ������������
	vector<glm::vec3> vertex_normals; // ���з���

public:
	ModelImporter() = default;
	void importModel(const char* filePath);
	int getNumVertices();
	vector<glm::vec3> getVertices();
};

#endif __MODELIMPORTER_H__