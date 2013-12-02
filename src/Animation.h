#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>

#define PI atan(1.0)*4.0

class Animation{
	std::string id;
	float span;
	bool doReset;

public:
	Animation(std::string id, float span);
	~Animation(){};

	virtual void apply(){};
	virtual void update(unsigned long sysTm){};
	virtual void addControlPoint(std::vector<float> pontos){};
	virtual void calculate(){};
	virtual void reset(){};

	std::string getId();
	float getSpan();
	bool getDoReset();

	void setId(std::string newId);
	void setSpan(float span);
	void setDoReset(bool r);
};


class LinearAnimation : public Animation{
	std::vector<std::vector<float>> controlPoints;

	float transX, transY, transZ, rot;
	float deltaX, deltaY, deltaZ, angle;
	int counter;
	float distance;

	unsigned int lastTime;
	float velocity;

public:
	LinearAnimation(std::string id,float span);

	void addControlPoint(std::vector<float> pontos);
	std::vector<std::vector<float>> getControlPoints();

	void apply();
	void update(unsigned long sysTm);
	void calculate();
	void reset();

	int getEstado();
	float getDeltaX();
	float getDeltaY();
	float getDeltaZ();
	float getRot();

	void setEstado(int estado);
	void setDeltaX(float deltaX);
	void setDeltaY(float deltaY);
	void setDeltaZ(float deltaZ);
	void setRot(float rot);

};

#endif