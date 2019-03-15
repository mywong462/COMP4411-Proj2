#include "marchingcube.h"
#include <iostream>
MarchingCube::MarchingCube(int noOfBox) : lengthOfBorder(noOfBox){
	//Calculate total number of vertex & box
	collectionOfVertex = new cubeVertex[(noOfBox + 1)*(noOfBox + 1)*(noOfBox + 1)];
	collectionOfCube = new cubeDetail[noOfBox*noOfBox*noOfBox];

	//Initialize collection of vertex
	int count = 0;
	for (int i = 0; i < noOfBox + 1; i++)
		for (int j = 0; j < noOfBox + 1; j++)
			for (int k = 0; k < noOfBox + 1; k++) {
				collectionOfVertex[count].position[0] = (static_cast<float>(i)/noOfBox * 2.0 - 1.0) * GRIDSIZE;
				collectionOfVertex[count].position[1] = (static_cast<float>(j) /noOfBox * 2.0 - 1.0) * GRIDSIZE;
				collectionOfVertex[count].position[2] = (static_cast<float>(k) /noOfBox * 2.0 - 1.0) * GRIDSIZE;
				collectionOfVertex[count].value = 0.0f;
				collectionOfVertex[count].normal[0] = collectionOfVertex[count].normal[1] = collectionOfVertex[count].normal[2] = 0;
				count++;
			}
	
	//Corresponding vertex of a box
	//	  7 - 6
	//	 /	 /|
	//	3 - 2 5
	//	|   |/
	//	0 - 1
	edgeVertexMapper[0][0] = 0;
	edgeVertexMapper[0][1] = 1;
	edgeVertexMapper[1][0] = 1;
	edgeVertexMapper[1][1] = 2;
	edgeVertexMapper[2][0] = 2;
	edgeVertexMapper[2][1] = 3;
	edgeVertexMapper[3][0] = 3;
	edgeVertexMapper[3][1] = 0;
	edgeVertexMapper[4][0] = 4;
	edgeVertexMapper[4][1] = 5;
	edgeVertexMapper[5][0] = 5;
	edgeVertexMapper[5][1] = 6;
	edgeVertexMapper[6][0] = 6;
	edgeVertexMapper[6][1] = 7;
	edgeVertexMapper[7][0] = 7;
	edgeVertexMapper[7][1] = 4;
	edgeVertexMapper[8][0] = 0;
	edgeVertexMapper[8][1] = 4;
	edgeVertexMapper[9][0] = 1;
	edgeVertexMapper[9][1] = 5;
	edgeVertexMapper[10][0] = 2;
	edgeVertexMapper[10][1] = 6;
	edgeVertexMapper[11][0] = 3;
	edgeVertexMapper[11][1] = 7;
	
	//Initialize collection of box
	count = 0;
	for (int i = 0; i < noOfBox; i++)
		for (int j = 0; j < noOfBox; j++)
			for (int k = 0; k < noOfBox; k++) {
				collectionOfCube[count].vertex[0] = &collectionOfVertex[k+(i*(noOfBox + 1) + j)*(noOfBox + 1) ];
				collectionOfCube[count].vertex[1] = &collectionOfVertex[(k+1)+(i*(noOfBox + 1) + j)*(noOfBox + 1)];
				collectionOfCube[count].vertex[2] = &collectionOfVertex[(k+1)+(i*(noOfBox + 1) + (j + 1))*(noOfBox + 1)];
				collectionOfCube[count].vertex[3] = &collectionOfVertex[k+(i*(noOfBox + 1) + (j + 1))*(noOfBox + 1)];
				collectionOfCube[count].vertex[4] = &collectionOfVertex[k+((i + 1)*(noOfBox + 1) + j)*(noOfBox + 1)];
				collectionOfCube[count].vertex[5] = &collectionOfVertex[(k+1)+((i + 1)*(noOfBox + 1) + j)*(noOfBox + 1)];
				collectionOfCube[count].vertex[6] = &collectionOfVertex[(k+1)+((i + 1)*(noOfBox + 1) + (j + 1))*(noOfBox + 1)];
				collectionOfCube[count].vertex[7] = &collectionOfVertex[k+((i + 1)*(noOfBox + 1) + (j + 1))*(noOfBox + 1)];
				count++;
			}
		
}

MarchingCube::~MarchingCube() {
	if (collectionOfCube)delete[] collectionOfCube;
	if (collectionOfVertex)delete[] collectionOfVertex;
	collectionOfCube = NULL;
	collectionOfVertex = NULL;
}

void MarchingCube::drawSurface(float iso) {
	cubeVertex surfaceVertice[12];
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < lengthOfBorder * lengthOfBorder * lengthOfBorder; i++) {
		unsigned char index = 0;

		if (collectionOfCube[i].vertex[0]->value < iso) index |= 1;
		if (collectionOfCube[i].vertex[1]->value < iso) index |= 2;
		if (collectionOfCube[i].vertex[2]->value < iso) index |= 4;
		if (collectionOfCube[i].vertex[3]->value < iso) index |= 8;
		if (collectionOfCube[i].vertex[4]->value < iso) index |= 16;
		if (collectionOfCube[i].vertex[5]->value < iso) index |= 32;
		if (collectionOfCube[i].vertex[6]->value < iso) index |= 64;
		if (collectionOfCube[i].vertex[7]->value < iso) index |= 128;
		if (index == 0 || index == 255) continue; // It means the point is either inside the range or out of bound

		int edgeToDraw = edgeTable[index];
		for (int selectedEdge = 0; selectedEdge < 12; selectedEdge++) {

			cubeVertex* vertexUn = collectionOfCube[i].vertex[edgeVertexMapper[selectedEdge][0]];
			cubeVertex* vertexDeux = collectionOfCube[i].vertex[edgeVertexMapper[selectedEdge][1]];

			//Calculate the position of the vertex forming the surface
			float diff = (iso - vertexUn->value) / (vertexDeux->value - vertexUn->value);

			surfaceVertice[selectedEdge].position[0] = vertexUn->position[0] + diff * (vertexDeux->position[0] - vertexUn->position[0]);
			surfaceVertice[selectedEdge].normal[0] = vertexUn->normal[0] + diff * (vertexDeux->normal[0] - vertexUn->normal[0]);
			surfaceVertice[selectedEdge].position[1] = vertexUn->position[1] + diff * (vertexDeux->position[1] - vertexUn->position[1]);
			surfaceVertice[selectedEdge].normal[1] = vertexUn->normal[1] + diff * (vertexDeux->normal[1] - vertexUn->normal[1]);
			surfaceVertice[selectedEdge].position[2] = vertexUn->position[2] + diff * (vertexDeux->position[2] - vertexUn->position[2]);
			surfaceVertice[selectedEdge].normal[2] = vertexUn->normal[2] + diff * (vertexDeux->normal[2] - vertexUn->normal[2]);
		}
		for (int j = 0; triTable[index][j * 3] != -1; j++) {
			glNormal3fv(surfaceVertice[triTable[index][j * 3]].normal);
			glVertex3fv(surfaceVertice[triTable[index][j * 3]].position);
			glNormal3fv(surfaceVertice[triTable[index][j * 3 + 1]].normal);
			glVertex3fv(surfaceVertice[triTable[index][j * 3 + 1]].position);
			glNormal3fv(surfaceVertice[triTable[index][j * 3 + 2]].normal);
			glVertex3fv(surfaceVertice[triTable[index][j * 3 + 2]].position);

		}


	}
	glEnd();

}