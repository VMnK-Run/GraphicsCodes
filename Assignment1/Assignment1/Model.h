#ifndef __MODEL_H__
#define __MODEL_H__
#include <glad/glad.h>                 //所有头文件 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION        //原作者没写
#include "stb_image.h"
#include <assimp/Importer.hpp>        //assimp库头文件
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

//Model类
class Model
{
public:
	/*  Model数据 */
	vector<Mesh> meshes;
	string directory;

	/*  函数  */
	// 构造函数,需要一个3D模型的文件路径
	Model(string const &path) {
		loadModel(path);
	}

	// 绘制模型，从而绘制所有网格
	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

private:
	/*  函数   */
	// 从文件加载支持ASSIMP扩展的模型，并将生成的网格存储在网格矢量中。
	void loadModel(string const &path) {
		// 通过ASSIMP读文件
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// 检查错误
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // 如果不是0
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// 检索文件路径的目录路径
		directory = path.substr(0, path.find_last_of('/'));

		// 以递归方式处理ASSIMP的根节点
		processNode(scene->mRootNode, scene);
	}

	// 以递归方式处理节点。 处理位于节点处的每个单独网格，并在其子节点（如果有）上重复此过程。
	void processNode(aiNode *node, const aiScene *scene) {
		// 处理位于当前节点的每个网格
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			// 节点对象仅包含索引用来索引场景中的实际对象。
			// 场景包含所有数据，节点只是为了有组织的保存东西（如节点之间的关系）。
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// 在我们处理完所有网格（如果有的话）后，我们会递归处理每个子节点
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
		// 要填写的数据
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		// 遍历每个网格的顶点
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
           // 我们声明一个占位符向量，因为assimp使用它自己的向量类，它不直接转换为glm的vec3类，所以我们首先将数据传递给这个占位符glm :: vec3。
			glm::vec3 vector; 
			// 位置
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// 法线
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			vertices.push_back(vertex);
		}
		//现在遍历每个网格面（一个面是一个三角形的网格）并检索相应的顶点索引。
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// 检索面的所有索引并将它们存储在索引向量中
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// 返回从提取的网格数据创建的网格对象
		return Mesh(vertices, indices);
	}

};
#endif