#ifndef TRANSFROMS_H
#define TRANSFROMS_H

#include <iostream>
#include "CGFscene.h"
#include "CGFappearance.h"
#include "CGFapplication.h"



class Transforms
{	
public:

	Transforms();
	virtual void apply();
	~Transforms();

};

class Translate:public Transforms
{
	float x;
	float y;
	float z;

public:
	Translate(float x, float y, float z); 
	void apply();
	
	float getX();
	float getY();
	float getZ();

	void setX(float x);
	void setY(float y);
	void setZ(float z);

};


class Scale:public Transforms
{
private:
	float x;
	float y;
	float z;

public:
	Scale(float x, float y, float z);
	void apply();
	
	float getX();
	float getY();
	float getZ();

	void setX(float x);
	void setY(float y);
	void setZ(float z);
};


class Rotate:public Transforms
{
private:
	float angle;
	std::string axis;

public:
	Rotate(float angle, string axis);
	void apply();

	float getAngle();
	std::string getAxis();

	void setAngle(float angle);
	void setAxis(string axis);
};

#endif

