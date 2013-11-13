#ifndef APPEARENCE_H
#define APPEARENCE_H

#include <string>
#include <CGFappearance.h>


class Appearence:public CGFappearance
{	
	std::string id, textureref;
	float emissive[4], textlength_s,textlength_t;

public:
	Appearence(std::string id,  float* emissive,float* ambient,float*diffuse,float* specular,float shininess, std::string textureref, float textlength_s, float textlength_t);
	Appearence(std::string id, float* emissive,float* ambient,float*diffuse,float* specular,float shininess);
	virtual ~Appearence();
	std::string getTextureref();
	float getTextlength_s();
	float getTextlength_t();
};
#endif