#pragma once
#include <vector>
#include <glm/glm.hpp>
using namespace std;

class BezierFace
{
	//顶点个数
	int numVertices;
	//索引个数
	int numIndices;
	//顶点
	vector<glm::vec3> vertices;
	//法线
	vector<glm::vec3> normals;
	//纹理
	vector<glm::vec2> texCoords;
	//索引
	vector<int> indices;
	//计算数据
	void init(int prec);
	//控制点
	float* controlPoints;
	vector<glm::vec3> controlPointsVector;
	//曲线阶数
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
