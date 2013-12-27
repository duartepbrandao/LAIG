#ifndef _XMLSCENE_H_
#define _XMLSCENE_H_

#include "tinyxml.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Globals.h"
#include "SpotLight.h"
#include "Camera.h"
#include "Primitive.h"
#include "Node.h"
#include "Transforms.h"
#include "Texture.h"
#include "Appearence.h"
#include <map>
#include "Animation.h"
#include "objload.h"

class XMLScene
{
public:
	static int num;
	//Globals
	Globals* globals;
	Globals* getGlobals();
	//Cameras
	vector<Camera*> camera;
	vector<Camera*> getCameras();
	//Lights
	vector<Light*> lights;
	vector<Light*> getLights();
	//Textures
	map<string,Texture*> textures;
	map<string,Texture*> getTextures();
	//Appearances
	map<string, Appearence*> appearances;
	map<string, Appearence*> getAppearances();
	//Animations
	map<string,Animation*> animations;
	map<string,Animation*> getAnimations();
	//Nodes
	std::string rootid;
	std::string getRootID();
	Node* node;
	map<string, Node*> scene;
	map<string, Node*> getScene();

	//Twixt
	Obj::File* obj;

	XMLScene(char *filename);
	~XMLScene();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	//methods to read xml tags
	std::string readString(char* attribute,TiXmlElement* element);
	int readBool(char* attribute, TiXmlElement* element);
	float readFloat(char* attribute,TiXmlElement* element);
	void read2Float(char* attribute,TiXmlElement* element, float* flt);
	void read3Float(char* attribute,TiXmlElement* element, float* flt);
	void read4Float(char* attribute,TiXmlElement* element, float* flt);

	void loadYAF(char* filename);
	void doInitElement();
	void doCameraElement();
	void doLightingElement();
	void doTextureElement();
	void doAppearanceElement();
	void doAnimationElement();
	void doGraphElement();

protected:

	TiXmlDocument* doc;

	//globals
	TiXmlElement* initElement; 

	//cameras
	TiXmlElement* cameraElement;
	TiXmlElement* perspectiveElement;
	TiXmlElement* orthoElement;

	//lights
	TiXmlElement* lightingElement;
	TiXmlElement* omniElement;
	TiXmlElement* spotElement;

	//texture
	TiXmlElement* textureElement;
	TiXmlElement* texturesElement;

	//Appearance
	TiXmlElement* appearanceElement;
	TiXmlElement* appearancesElement;

	//Animations
	TiXmlElement* animationsElement;
	TiXmlElement* animationElement;

	//Graph
	TiXmlElement* graphElement;
	TiXmlElement* nodeElement;
	TiXmlElement* transformsElement;
	TiXmlElement* transformElement;
	TiXmlElement* translateElement;
	TiXmlElement* rotateElement;
	TiXmlElement* scaleElement;
	TiXmlElement* appearancerefElement;
	TiXmlElement* animationrefElement;
	TiXmlElement* childrenElement;
	TiXmlElement* noderefElement;
	TiXmlElement* rectangleElement;
	TiXmlElement* sphereElement;
	TiXmlElement* torusElement;
	TiXmlElement* cylinderElement;
	TiXmlElement* triangleElement;

	TiXmlElement* waterlineElement;
	TiXmlElement* patchElement;
	TiXmlElement* planeElement;
	TiXmlElement* controlElement;
	TiXmlElement* controlPointElement;

	TiXmlElement* objectElement;
};

#endif