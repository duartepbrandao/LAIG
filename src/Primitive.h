#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <CGFshader.h>
#include <CGFtexture.h>
#include "Appearence.h"
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>


#define pi atan(1.0)*4.0

class Primitive
{
public:
	float s, t;
	Primitive();
	~Primitive();
	virtual void draw();
	virtual void update(unsigned long systm){};
	void setS(float s);
	void setT(float t);
	virtual void initShaders();
};

//torus
class Ptorus:public Primitive
{
	float inner, outer, slices, stacks;
	unsigned int theTorus;

public:
	Ptorus(float inner, float outer, float slices, float stacks);
	~Ptorus();
	void draw();
};

//rectangle
class Prectangle:public Primitive
{
	float x1, y1, x2, y2,x3,y3, x4, y4;
	unsigned int theRectangle;
public:
	Prectangle(float x1, float y1, float x2, float y2);
	~Prectangle();
	void calcValues();
	void draw();
};

//cylinder
class Pcylinder:public Primitive
{
	float base, top, height, slices, stacks;
	unsigned int theCylinder;
public:
	Pcylinder(float base, float top, float height, float slices, float stacks);
	~Pcylinder(void);
	void drawCircle(float radius);
	void draw();
};

//sphere
class Psphere:public Primitive
{
	float radius, slices, stacks;
	unsigned int theSphere;
public:
	Psphere(float radius, float slices, float stacks);
	~Psphere(void);
	void draw();
};

//triangle
class Ptriangle:public Primitive
{
	float n1, n2, n3, p0[2], p1[2], p2[2];
	float x1, y1, x2, y2,x3,y3,z1,z2,z3;
	unsigned int theTriangle;
public:
	Ptriangle(float x1,float y1,float z1, float x2,float y2, float z2, float x3, float y3,float z3);
	~Ptriangle();
	void draw();
	void calcValues();
	void calculateNormal();
};

//waterline


//plane
class Pplane:public Primitive
{
	float parts;

public:
	Pplane(int parts);
	float getParts();
	void draw();
};

class Ppatch:public Primitive
{
	int order, partsU, partsV;
	std::string compute;
	float grid_1[4][3];
	float grid_2[9][3];
	float grid_3[16][3];

public:
	Ppatch(int order, int partsU, int partsV, std::string compute);
	~Ppatch();
	void addElementToGrid(int i, int j, float value);
	void draw();
};



class Pwaterline:public Primitive, CGFshader
{	
private:
	Pplane* plano;
	Pcylinder* stag;
	std::string heightmap;
	std::string texturemap;
	std::string fragmentshader;
	std::string vertexshader;
	bool firstTime;
	string vert,frag;
	float timer, timerL, lastTime;
	CGFtexture * baseTexture;
	CGFtexture * secTexture;
	GLint baseImageLoc;
	GLint secImageLoc;


public:
	Pwaterline(void);
	Pwaterline(std::string heightmap, std::string texturemap, std::string fragmentshader, std::string vertexshader);
	void setHeightMap(std::string height);
	std::string getHeightMap();
	std::string getTextureMap();
	void setTextureMap(std::string texture);
	std::string getFragmentShader();
	void setFragmentShader(std::string fragment);
	std::string getVertexShader();
	void setVertexShader(std::string vertex);
	void draw();
	void update(unsigned long systm);
};
#endif