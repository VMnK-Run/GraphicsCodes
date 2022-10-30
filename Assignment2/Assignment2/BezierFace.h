#pragma once
#include <vector>
#include <glm/glm.hpp>
using namespace std;

class BezierFace
{
	//�������
	int numVertices;
	//��������
	int numIndices;
	//����
	vector<glm::vec3> vertices;
	//����
	vector<glm::vec3> normals;
	//����
	vector<glm::vec2> texCoords;
	//����
	vector<int> indices;
	//��������
	void init(int prec);
	//���Ƶ�
	float* controlPoints;
	vector<glm::vec3> controlPointsVector;
	//���߽���
	int step;
	float toRadians(float degrees);
	float Bernstein(float u, int index);
public:
	BezierFace() = default;
	BezierFace(int step, float controlPoints[], int prec);
	int getNumVertices();
	int getNumIndices();
	vector<glm::vec3> getVertices();
	vector<glm::vec3> getNormals();
	vector<glm::vec2> getTexCoords();
	vector<int> getIndices();

};
