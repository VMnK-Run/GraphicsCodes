#ifndef __MESH_H__
#define __MESH_H__
#include <glad/glad.h> // 所有头文件
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
//顶点
struct Vertex {
	// 位置
	glm::vec3 Position;
	// 法向量
	glm::vec3 Normal;
};

//Mesh类
class Mesh {
public:
	/*  Mesh 数据  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int VAO;

	/*  函数  */
	// 构造函数 参数：顶点 索引
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices) {
		this->vertices = vertices;
		this->indices = indices;
		// 现在我们拥有了所有必需的数据，设置顶点缓冲区及其属性指针。
		setupMesh();
	}

	// 画网格模型
	void Draw(Shader shader) {
		// 画网格
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// 一旦配置完毕将一切设置回默认值总是很好的做法，。
		glActiveTexture(GL_TEXTURE0);
	}

private:
	/*  渲染数据  */
	unsigned int VBO, EBO;

	/*  函数    */
	// 初始化所有缓冲区对象/数组
	void setupMesh() {
		// 创建缓冲区/数组
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		//将数据加载到顶点缓冲区中
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 关于结构的一个好处是它们的内存布局对于它的所有项都是顺序的。
		// 结果是我们可以简单地将指针传递给结构，并且它完美地转换为glm :: vec3 / 2数组，该数组再次转换为3/2浮点数，转换为字节数组。
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		// 设置顶点属性指针
		// 顶点位置
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// 顶点法线
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	}
};
#endif