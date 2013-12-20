#ifndef LightingScene_H
#define LightingScene_H

#include "CGFscene.h"
#include "CGFappearance.h"
#include "XMLScene.h"
#include "Primitive.h"
#include "Node.h"
#include "Animation.h"
#include "objload.h"


class LightingScene : public CGFscene
{
public:
	float deltaX, deltaY, x, y;

	void init();
	void display();

	//GLOBALS
	Globals* globals;
	int drawmode;
	//CAMERAS
	vector<Camera*> cameras;
	int camera;
	//LIGHTS
	vector<int> lightState;
	vector<Light*> lights;
	//GRAPH
	std::string id;
	map<string, Node*> scene;
	//APPEARANCES
	int app;
	bool appChoose;
	float s, t;
	map<string, Appearence*> appearances;
	//ANIMATIONS
	map<string, Animation*> animations;
	Animation* animation;

	//Twixt
	Node* n;
	Peca* peca;
	CGFobject* object;//picking
	vector<Appearence*> apps;
	vector<Peca> pecas;

	LightingScene(vector<Light*> lights,Globals* globals, vector<Camera*> camera, std::string id, map<string, Node*> scene, map<string, Appearence*> appearances, map<string, Animation*> animations);
	~LightingScene();
	void setLightState(int pos, int value);
	int getLightState(int pos);

	void initDisplayLists(string id);
	void drawPrimitives(string id);
	void drawAllThings(string id);
	void applyTextures(string id);

	void update(unsigned long sysTm);

	void loadTheme(char* filename);
};

#endif