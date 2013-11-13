#include "Globals.h"


Globals::Globals(float* background, std::string drawmode,std::string  shading, std::string cullface,std::string  cullorder){
	for(int i=0;i<4;i++){
		this->background[i]=background[i];
	}
	this->drawmode=setValue(drawmode);
	this->shading=setValue(shading);
	this->cullface=setValue(cullface);
	this->cullorder=setValue(cullorder);
}


Globals::~Globals(void)
{
}

int Globals::setValue(std::string s){
	if(s=="fill"){
		return GL_FILL;
	}else if(s=="line"){
		return GL_LINE;
	}else if(s=="point"){
		printf("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		return GL_POINT;
	}else if(s=="flat"){
		return GL_FLAT;
	}else if(s=="gouraud"){
		return GL_SMOOTH;
	}else if(s=="none"){
		return GL_NONE;
	}else if(s=="back"){
		return GL_BACK;
	}else if(s=="front"){
		return GL_FRONT;
	}else if(s=="both"){
		return GL_FRONT_AND_BACK;
	}else if(s=="CCW"){
		return GL_CCW;
	}
	return GL_CW;
}

float* Globals::getBackgroundPointer(){
	return background;
}

float Globals::getBackground(int i){
	return background[i];
}
	
int Globals::getDrawMode(){
	return drawmode;
}
	
int Globals::getShading(){
	return shading;
}
	
int Globals::getCullface(){
	return cullface;
}
	
int Globals::getCullorder(){
	return cullorder;
}
