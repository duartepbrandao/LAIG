#ifndef Object_H
#define Object_H

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "Primitive.h"



class Object : public Primitive{
public:
	Object(std::string filename);
	const aiScene* getScene();
	void draw();
	~Object();
private:
	const aiScene* scene;
	Assimp::Importer importer;
	void recursive_render(const struct aiScene *sc, const struct aiNode* nd);
};
#endif