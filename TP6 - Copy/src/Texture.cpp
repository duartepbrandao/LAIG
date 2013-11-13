#include "Texture.h"


Texture::Texture(std::string id,std::string path){
	this->id=id;
	this->path=path;
}

Texture::~Texture(){
}

std::string Texture::getPath(){
	return path;
}
std::string Texture::getID(){
	return id;
}