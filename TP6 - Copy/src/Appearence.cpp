#include "Appearence.h"


Appearence::Appearence(string id,  float* emissive,float* ambient,float*diffuse,float* specular,float shininess, string textureref, float textlength_s, float textlength_t):CGFappearance(ambient, diffuse, specular, shininess){
	this->id=id;
	this->textureref=textureref;
	this->textlength_s=textlength_s;
	this->textlength_t=textlength_t;
	//setTexture (textureref);
	//setTextureWrap (textlength_s, texlength_t);
	for(int i=0;i<4;i++){
		this->emissive[i]=emissive[i];
	}
}

Appearence::Appearence(string id,  float* emissive,float* ambient,float*diffuse,float* specular,float shininess):CGFappearance(ambient, diffuse, specular, shininess){
	this->id=id;
	for(int i=0;i<4;i++){
		this->emissive[i]=emissive[i];
	}
}

Appearence::~Appearence(){
}
	
string Appearence::getTextureref(){
	return textureref;
}

float Appearence::getTextlength_s(){
	return textlength_s;
}
	
float Appearence::getTextlength_t(){
	return textlength_t;
}