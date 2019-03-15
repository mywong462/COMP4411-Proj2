#ifndef METABALL_H
#define METABALL_H
#include "marchingcube.h"
#include <vector>

struct Ball {
	float position[3];
	float radius;
	Ball(float pos[3], float radius) {
		position[0] = pos[0];
		position[1] = pos[1];
		position[2] = pos[2];
		this->radius = radius;
	}
};

class MetaBall {
public:
	MetaBall();
	~MetaBall();
	void draw(float isoValue);
	void addBall(float x, float y, float z, float radius);

private:
	std::vector<Ball> collectionOfBall;
	MarchingCube* algor;

};
#endif