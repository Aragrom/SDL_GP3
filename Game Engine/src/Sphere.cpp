/*
==================================================================================
cSphere.cpp
==================================================================================
*/

#include "GameConstants.h"
#include <math.h>
#include <cmath>

Sphere::Sphere(Mesh *mesh, float radius, unsigned int rings, unsigned int sectors)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texcoords.resize(rings * sectors * 2);
	std::vector<GLfloat>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	std::vector<GLfloat>::iterator t = texcoords.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-1.57079632679489661923132169164 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*t++ = s*S;
		*t++ = r*R;

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

		*n++ = x;
		*n++ = y;
		*n++ = z;
	}

	indices->resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices->begin();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*i++ = (r + 1) * sectors + s;
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = r * sectors + (s + 1);
		*i++ = r * sectors + s;
	}

	int iVertexCount = getVertexCount();
	vertexPosition = new Vertex[iVertexCount];
	formatVertexData();
	mesh->copyVertexData(iVertexCount, sizeof(Vertex), (void**)vertexPosition);

	int iIndicesCount = indices->capacity();
	mesh->copyIndexData(iIndicesCount, sizeof(int), (void**)indices);
}

void Sphere::formatVertexData() {

	int iVertexIndex = 0;
	int iVectorCount = 0;
	vec3 v3Position = vec3();
	for (auto iter = vertices.begin(); iter != vertices.end(); iter++)
	{
		iVectorCount++;
		switch (iVectorCount) {
		case 0:
			v3Position.x = (*iter);
			break;
		case 1:
			v3Position.y = (*iter);
			break;
		case 2:
			v3Position.z = (*iter);
			break;
		default:
			std::cout << "Default state reached in Sphere.cpp" << std::endl;
			break;
		}

		if (iVectorCount == 3) {

			vertexPosition[iVertexIndex].position = v3Position;
			iVertexIndex++;
			iVectorCount = 0;
		}
	}
}

int Sphere::getVertexCount()
{
	int iVertex;
	int iVector = 0;

	for (auto iter = vertices.begin(); iter != vertices.end(); iter++)
	{
		iVector++;
		if (iVector == 3)
		{
			iVertex++;
			iVector = 0;
		}
	}

	return iVertex;
}