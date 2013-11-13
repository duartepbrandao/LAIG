#include "SpotLight.h"

SpotLight::SpotLight(float angle, float exp,std::string id,bool doub, bool loc, bool enab, float* ambi,float *amb, float *spec, float *diff,bool enable, unsigned int lightid, float *pos,float*dir=NULL):Light(id, doub, loc, enab, ambi, amb, spec, diff, enable, lightid, pos, dir){
	this->angle=angle;
	this->exp=exp;
	for(int i=0;i<4;i++){
		this->direction[i]=dir[i];
	}
	id=lightid;
}

SpotLight::~SpotLight(){

}

float SpotLight::getAngle(){
	return angle;
}

float* SpotLight::getDirection(){
	return direction;
}
	
float SpotLight::getExponent(){
	return exp;
}

void SpotLight::setVariables(){
	Light::setVariables();
	glLightf(id, GL_SPOT_CUTOFF, angle);
	glLightf(id, GL_SPOT_EXPONENT, exp);
	glLightfv(id, GL_SPOT_DIRECTION, direction);
}
