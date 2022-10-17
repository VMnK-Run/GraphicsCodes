#ifndef __MODEL_H__
#define __MODEL_H__
#include <glad/glad.h>                 //����ͷ�ļ� 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION        //ԭ����ûд
#include "stb_image.h"
#include <assimp/Importer.hpp>        //assimp��ͷ�ļ�
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

//Model��
class Model
{
public:
	/*  Model���� */
	vector<Mesh> meshes;
	string directory;

	/*  ����  */
	// ���캯��,��Ҫһ��3Dģ�͵��ļ�·��
	Model(string const &path) {
		loadModel(path);
	}

	// ����ģ�ͣ��Ӷ�������������
	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

private:
	/*  ����   */
	// ���ļ�����֧��ASSIMP��չ��ģ�ͣ��������ɵ�����洢������ʸ���С�
	void loadModel(string const &path) {
		// ͨ��ASSIMP���ļ�
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// ������
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // �������0
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// �����ļ�·����Ŀ¼·��
		directory = path.substr(0, path.find_last_of('/'));

		// �Եݹ鷽ʽ����ASSIMP�ĸ��ڵ�
		processNode(scene->mRootNode, scene);
	}

	// �Եݹ鷽ʽ����ڵ㡣 ����λ�ڽڵ㴦��ÿ���������񣬲������ӽڵ㣨����У����ظ��˹��̡�
	void processNode(aiNode *node, const aiScene *scene) {
		// ����λ�ڵ�ǰ�ڵ��ÿ������
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			// �ڵ��������������������������е�ʵ�ʶ���
			// ���������������ݣ��ڵ�ֻ��Ϊ������֯�ı��涫������ڵ�֮��Ĺ�ϵ����
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// �����Ǵ�����������������еĻ��������ǻ�ݹ鴦��ÿ���ӽڵ�
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
		// Ҫ��д������
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		// ����ÿ������Ķ���
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
           // ��������һ��ռλ����������Ϊassimpʹ�����Լ��������࣬����ֱ��ת��Ϊglm��vec3�࣬�����������Ƚ����ݴ��ݸ����ռλ��glm :: vec3��
			glm::vec3 vector; 
			// λ��
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// ����
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			vertices.push_back(vertex);
		}
		//���ڱ���ÿ�������棨һ������һ�������ε����񣩲�������Ӧ�Ķ���������
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// ����������������������Ǵ洢������������
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// ���ش���ȡ���������ݴ������������
		return Mesh(vertices, indices);
	}

};
#endif