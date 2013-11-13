#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>



class Texture
{

	std::string id,path;

public:
	Texture(std::string id,std::string path);
	~Texture();
	std::string getPath();
	std::string getID();
	
};

#endif