#include "BezierFace.h"

void BezierFace::init(int prec)
{
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++)
	{
		vertices.push_back(glm::vec3());
		normals.push_back(glm::vec3());
		texCoords.push_back(glm::vec2());
	}
	for (int i = 0; i < numIndices; i++)
	{
		indices.push_back(0);
	}
	for (int i = 0; i < (step + 1) * (step + 1) * 3; i += 3)
	{
		controlPointsVector.push_back(glm::vec3(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2]));
	}
	for (int i = 0; i <= prec; i++)
	{
		for (int j = 0; j <= prec; j++)
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float u = (float)i / prec;
			float v = (float)j / prec;

			for (int ii = 0; ii <= step; ii++)
			{
				for (int jj = 0; jj <= step; jj++)
				{
					int index = ii * (step + 1) + jj;
					x += controlPointsVector[index].x * Bernstein(u, ii) * Bernstein(v, jj);
					y += controlPointsVector[index].y * Bernstein(u, ii) * Bernstein(v, jj);
					z += controlPointsVector[index].z * Bernstein(u, ii) * Bernstein(v, jj);
				}
			}
			vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
			normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
			texCoords[i * (prec + 1) + j] = glm::vec2((float)j / prec, (float)i / prec);
		}
	}
	//¼ÆËãË÷Òý
	for (int i = 0; i < prec; i++) {
		for (int j = 0; j < prec; j++) {
			indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
			indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
			indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
			indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
			indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
			indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
		}
	}
}
float BezierFace::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

float BezierFace::Bernstein(float t, int index) {
	switch (index)
	{
	default:
	case 0:
		return pow(1.0 - t, 4);
		break;
	case 1:
		return 4 * t * pow(1.0 - t, 3);
		break;
	case 2:
		return 6 * pow(t, 2) * pow(1.0 - t, 2);
		break;
	case 3:
		return 4 * pow(t, 3) * (1.0 - t);
		break;
	case 4:
		return pow(t, 4);
		break;
	}
}

BezierFace::BezierFace(int step, float controlPoints[], int prec)
{
	this->step = step;
	this->controlPoints = controlPoints;
	init(prec);
}

int BezierFace::getNumVertices()
{
	return numVertices;
}

int BezierFace::getNumIndices()
{
	return numIndices;
}

vector<glm::vec3> BezierFace::getVertices()
{
	return vertices;
}

vector<glm::vec3> BezierFace::getNormals()
{
	return normals;
}

vector<glm::vec2> BezierFace::getTexCoords()
{
	return texCoords;
}

vector<int> BezierFace::getIndices()
{
	return indices;
}

