#include "metaball.h"
#include "modelerdraw.h"
#include <iostream>

MetaBall::MetaBall()
{
	ModelerDrawState *mds = ModelerDrawState::Instance();
	switch (mds->m_drawMode)
	{
	case NORMAL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_SMOOTH);
		break;
	case FLATSHADE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		break;
	case WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glShadeModel(GL_FLAT);
	default:
		break;
	}

	int divisions; 
	
	switch (mds->m_quality)
	{
	case HIGH:
		divisions = 32; break;
	case MEDIUM:
		divisions = 40; break;
	case LOW:
		divisions = 12; break;
	case POOR:
		divisions = 8; break;
	}
	collectionOfBall.clear();
	algor = new MarchingCube(divisions);
	
}

MetaBall::~MetaBall()
{
	collectionOfBall.clear();
	delete algor;
}

void MetaBall::draw(float isoValue)
{
	for (int i = 0; i < collectionOfBall.size(); i++) {
		float tempPos[3];
		int temp = algor->lengthOfBorder*algor->lengthOfBorder*algor->lengthOfBorder;
		for (int j = 0; j < temp; j++) {
			tempPos[0] = algor->collectionOfVertex[j].position[0] - collectionOfBall[i].position[0];
			tempPos[1] = algor->collectionOfVertex[j].position[1] - collectionOfBall[i].position[1];
			tempPos[2] = algor->collectionOfVertex[j].position[2] - collectionOfBall[i].position[2];
			float squaredDistance = tempPos[0] * tempPos[0] + tempPos[1] * tempPos[1] + tempPos[2] * tempPos[2];
			if (squaredDistance == 0.0f)
				squaredDistance = 0.00001f;

			algor->collectionOfVertex[j].value += collectionOfBall[i].radius*collectionOfBall[i].radius / squaredDistance;
			float tempNormal = collectionOfBall[i].radius*collectionOfBall[i].radius / (squaredDistance*squaredDistance);

			algor->collectionOfVertex[j].normal[0] += tempPos[0] * tempNormal;
			algor->collectionOfVertex[j].normal[1] += tempPos[1] * tempNormal;
			algor->collectionOfVertex[j].normal[2] += tempPos[2] * tempNormal;
		}
	}
	algor->drawSurface(isoValue);
}

void MetaBall::addBall(float x, float y, float z, float radius)
{
	float temp[3] = { x,y,z };
	collectionOfBall.push_back(Ball(temp, radius));

}
