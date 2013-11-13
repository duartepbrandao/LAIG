#include "Light.h"

bool Light::doublesided=0;
bool Light::local=0;
bool Light::enabled=0;
float Light::ambient[4]={0.0,0.0,0.0,0.0};

Light::Light(std::string id,bool doub, bool loc, bool enab, float* ambi,float *amb, float *spec, float *diff,bool enable, unsigned int lightid, float *pos,float*dir=NULL):CGFlight(lightid, pos,dir){
	//Parametros locais
	this->id=id;
	for(int i=0;i<4;i++){
		this->amb[i]=amb[i];
		this->spec[i]=spec[i];
		this->diff[i]=diff[i];
	}
	if(enable){
		this->enable();
	}else{
		this->disable();
	}
	//this->update();
	//Parametros globais
	for(int i=0;i<4;i++){
		ambient[i]=ambi[i];
	}
	doublesided=doub;
	local=loc;
	enabled=enab;
}

Light::~Light(){

}

bool Light::getDoublesided(){
	return doublesided;
}
	
bool Light::getLocal(){
	return local;
}
	
bool Light::getEnabled(){
	return enabled;
}
	
float* Light::getAmbient(){
	return ambient;
}

std::string Light::getID(){
	return id;
}
float* Light::getAmb(){
	return amb;
}

float* Light::getSpecular(){
	return specular;
}
	
float* Light::getDiffuse(){
	return diffuse;
}

void Light::setVariables(){
	this->setAmbient(amb);
	this->setSpecular(specular);
	this->setDiffuse(diffuse);
}
