#ifndef __MESH_H__
#define __MESH_H__
#include <glad/glad.h> // ����ͷ�ļ�
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
//����
struct Vertex {
	// λ��
	glm::vec3 Position;
	// ������
	glm::vec3 Normal;
};

//Mesh��
class Mesh {
public:
	/*  Mesh ����  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int VAO;

	/*  ����  */
	// ���캯�� ���������� ����
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices) {
		this->vertices = vertices;
		this->indices = indices;
		// ��������ӵ�������б�������ݣ����ö��㻺������������ָ�롣
		setupMesh();
	}

	// ������ģ��
	void Draw(Shader shader) {
		// ������
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// һ��������Ͻ�һ�����û�Ĭ��ֵ���Ǻܺõ���������
		glActiveTexture(GL_TEXTURE0);
	}

private:
	/*  ��Ⱦ����  */
	unsigned int VBO, EBO;

	/*  ����    */
	// ��ʼ�����л���������/����
	void setupMesh() {
		// ����������/����
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		//�����ݼ��ص����㻺������
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// ���ڽṹ��һ���ô������ǵ��ڴ沼�ֶ��������������˳��ġ�
		// ��������ǿ��Լ򵥵ؽ�ָ�봫�ݸ��ṹ��������������ת��Ϊglm :: vec3 / 2���飬�������ٴ�ת��Ϊ3/2��������ת��Ϊ�ֽ����顣
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		// ���ö�������ָ��
		// ����λ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// ���㷨��
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	}
};
#endif