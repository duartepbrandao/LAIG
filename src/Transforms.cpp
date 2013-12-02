#include "Transforms.h"


Transforms::Transforms()
{
}


Transforms::~Transforms()
{
}

void Transforms::apply(){
}

Translate::Translate(float x, float y, float z){
	this->x=x;
	this->y=y;
	this->z=z;
}

void Translate::apply(){
	glTranslated(x,y,z);
}

float Translate::getX(){
	return x;
}
	
float Translate::getY(){
	return y;
}
	
float Translate::getZ(){
	return z;
}

void Translate::setX(float x){
	this->x=x;
}
	
void Translate::setY(float y){
	this->y=y;
}
	
void Translate::setZ(float z){
	this->z=z;
}

Scale::Scale(float x, float y, float z){
	this->x=x;
	this->y=y;
	this->z=z;
}

void Scale::apply(){
	glScaled(x,y,z);
}

float Scale::getX(){
	return x;
}
	
float Scale::getY(){
	return y;
}
	
float Scale::getZ(){
	return z;
}

void Scale::setX(float x){
	this->x=x;
}
	
void Scale::setY(float y){
	this->y=y;
}
	
void Scale::setZ(float z){
	this->z=z;
}

Rotate::Rotate(float angle, string axis){
	this->angle=angle;
	this->axis=axis;
}
	
void Rotate::apply(){
	if(axis=="x"){
		glRotated(angle,1,0,0);
	}else if(axis=="y"){
		glRotated(angle,0,1,0);
	}else if(axis=="z"){
		glRotated(angle,0,0,1);
	}
}

float Rotate::getAngle(){
	return angle;
}

string Rotate::getAxis(){
	return axis;
}

void Rotate::setAngle(float angle){
	this->angle=angle;
}
	
void Rotate::setAxis(string axis){
	this->angle=angle;
}
