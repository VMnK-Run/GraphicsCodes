#pragma once
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
	// 纹理坐标
	glm::vec2 TexCoords;
	// u向量
	glm::vec3 Tangent;
	// v向量
	glm::vec3 Bitangent;
};
//纹理
struct Texture {
	unsigned int id;
	string type;
	string path;
};
//Mesh类
class Mesh {
public:
	/*  Mesh 数据  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	/*  函数  */
	// 构造函数 参数：顶点 索引 纹理
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		// 现在我们拥有了所有必需的数据，设置顶点缓冲区及其属性指针。
		setupMesh();
	}

	// 画网格模型
	void Draw(Shader shader)
	{
		// 绑定适当的纹理
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // 绑定前激活适当的纹理单元
											  // 获取纹理编号（diffuse_textureN中的N）
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); 
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); 
			else if (name == "texture_height")
				number = std::to_string(heightNr++);
		    // 现在将采样器设置为正确的纹理单元
			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			// 最后绑定纹理
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
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
	void setupMesh()
	{
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
		// 顶点纹理坐标
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// u向量
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// v向量
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		glBindVertexArray(0);
	}
};