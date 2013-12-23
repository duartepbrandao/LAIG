#include "LightingScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "ExampleObject.h"

#define BOARD_HEIGHT 6.0
#define BOARD_WIDTH 6.4
#define NUM_OBJS 12*12
#define NUM_ROWS 12
#define NUM_COLS 12

LightingScene::LightingScene(vector<Light*> lights, Globals* globals,vector<Camera*> cameras, std::string id, map<string, Node*> scene, map<string, Appearence*> appearances,  map<string, Animation*> animations){
	this->globals=globals;
	this->lights=lights;
	//cameras
	this->cameras=cameras;
	for(unsigned int i=0; i<cameras.size();i++){
		if(cameras[0]->getInicial()==cameras[i]->getID()){
			camera=i;
		}
	}
	scene_cameras.insert(scene_cameras.end(),cameras.begin(),cameras.end());
	this->id=id;
	this->scene=scene;
	this->app=0;
	this->s=1;
	this->t=1;
	this->appChoose=true;
	this->appearances=appearances;
	this->animations=animations;
	drawmode=this->globals->getDrawMode()-GL_POINT;

	/*x=deltaX=3.05+1.5/2;
	y=deltaY=3.1+1.5/2;*/

	c1 = new Peca("Classic/classic.yaf", 1, "Classic/player1.jpg", "Classic/player1.jpg");
	c2 = new Peca("Classic/classic.yaf", 2, "Classic/player2.jpg", "Classic/player2.jpg");
	m1 = new Peca("Mario/KoopaTropa.obj","Mario/mario.yaf", 1, "Mario/KoopaTropa.jpg", "Mario/base_2.jpg");
	m2 = new Peca("Mario/goomba.obj","Mario/mario.yaf", 2, "Mario/goomba.jpg", "Mario/base.jpg");
	d1 = new Peca("DragonBall/dragonball.yaf", 1, "DragonBall/player1.jpg", "DragonBall/player1.jpg");
	d2 = new Peca("DragonBall/dragonball.yaf", 2, "DragonBall/player2.jpg", "DragonBall/player2.jpg");
	a1 = new Peca("AngryBirds/red.obj","AngryBirds/angrybirds.yaf", 1, "AngryBirds/red.jpg", "AngryBirds/base.jpg");
	a2 = new Peca("AngryBirds/pig.obj","AngryBirds/angrybirds.yaf", 2, "AngryBirds/pig.jpg", "AngryBirds/base_2.jpg");

	/*peca=new Peca("Classic/classic.yaf", 1, "Classic/topo_1.jpg", "Classic/base.jpg");*/
	object=new ExampleObject();
}

void LightingScene::init() 
{


	//GLOBALS
	glClearColor (globals->getBackground(0), globals->getBackground(1), globals->getBackground(2), globals->getBackground(3));
	glPolygonMode(GL_FRONT_AND_BACK,globals->getDrawMode());
	glShadeModel (globals->getShading());
	glFrontFace(globals->getCullorder());
	glCullFace(globals->getCullface());

	//CAMERAS
	//activateCamera(camera);

	//LIGHTS
	if(Light::getEnabled()){
		glEnable(GL_LIGHTING);
	}else{
		glDisable(GL_LIGHTING);
	}
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, Light::getDoublesided());
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,Light::getLocal());
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light::getAmbient());

	if(!lights.empty()){
		for(unsigned int i=0;i<lights.size();i++){
			lights[i]->setVariables();
			lightState.push_back(lights[i]->getEnabled());
		}
	}

	//set Textures
	applyTextures(id);

	//initialize display Lists
	initDisplayLists(id);

	glEnable (GL_NORMALIZE);

	setUpdatePeriod(10);
}

void LightingScene::display() 
{

	// ---- BEGIN Background, camera and axis setup
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();
	activateCamera(camera);
	activeCamera->applyView();
	//cameras

	//GLOBALS
	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT+drawmode);

	//LIGHTS
	for(int i=0;i<lightState.size();i++){
		if(lightState[i]==0){
			lights[i]->disable();
		}else{
			lights[i]->enable();
		}
		lights[i]->draw();
		lights[i]->update();
	}


	// Draw axis
	axis.draw();
	// ---- END Background, camera and axis setup

	// ---- BEGIN Primitive drawing section
	drawAllThings(id);
	for(unsigned int i=0; i<pecas.size();i++){
		glPushMatrix();
		glTranslated(pecas[i]->getPosX(), 0.0, pecas[i]->getPosY());
		pecas[i]->draw();
		glPopMatrix();
	}
	// ---- END Primitive drawing section

	GLint mode;
	glGetIntegerv(GL_RENDER_MODE, &mode);
	if(mode!=GL_RENDER){
		//Picking
		// Example 1: simple naming
		glPushMatrix();

		glPushName(-1);		// Load a default name

		for (int i=0; i< NUM_OBJS;i++)
		{
			glPushMatrix();
			glLoadName(i);		//replaces the value on top of the name stack
			glPopMatrix();
		}
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		for (int r=0; r < NUM_ROWS; r++)
		{
			glPushMatrix();
			glScaled(1.5,1.0,1.5);
			glTranslatef(r*2.57+3.05, 0.01, 0.0);
			glRotated(90, 0, 0, 1);
			glLoadName(r);
			for (int c=0; c < NUM_COLS; c++)
			{
				glPushMatrix();
				glTranslatef(0,0.01,c*2.57+3.1);
				glRotatef(90,0,1,0);
				glPushName(c);
				object->draw();
				glPopName();
				glPopMatrix();
			}
			glPopMatrix();
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		}
	}
	// ---- END feature demos

	//change theme
	if(!appChoose){
		cameras.clear();
		lightState.clear();
		lights.clear();
		scene.clear();
		appearances.clear();
		animations.clear();
		
		loadTheme(app);
		loadPecaTheme();
		init();
		appChoose=true;
	}

	glutSwapBuffers();
}

void LightingScene::applyTextures(string id){
	Node* node=this->scene[id];

	if(node->getAppID()!=""){
		s=appearances[node->getAppID()]->getTextlength_s();
		t=appearances[node->getAppID()]->getTextlength_t();
		appearances[node->getAppID()]->setTexture(appearances[node->getAppID()]->getTextureref());
		appearances[node->getAppID()]->setTextureWrap(s, t);
	}
	if(node->getPrimitives().size()!=0){
		for(unsigned int j=0;j<node->getPrimitives().size();j++){
			node->getPrimitives()[j]->setS(s);
			node->getPrimitives()[j]->setT(t);
		}
	}else{
		for(unsigned int j=0;j<node->getDescendents().size();j++){
			applyTextures(node->getDescendents()[j]);
		}
	}
}


void LightingScene::loadTheme(int num){
	//XMLScene xmlscene=XMLScene(filename);
	this->lights= themes[num]->getLights();
	this->globals=themes[num]->getGlobals();
	this->cameras=themes[num]->getCameras();
	this->id=themes[num]->getId();
	this->scene=themes[num]->getScene();
	this->appearances=themes[num]->getAppearances();
	this->animations=themes[num]->getAnimations();
}

void LightingScene::loadPecaTheme(){

	for(int i=0;i<pecas.size();i++){
		if(app==0){
			if(pecas[i]->getPlayerNumber()==1){
				pecas[i]->changePecaTheme(c1);
			}else{
				pecas[i]->changePecaTheme(c2);
			}
		}else if(app==1){
			if(pecas[i]->getPlayerNumber()==1){
				pecas[i]->changePecaTheme(m1);
			}else{
				pecas[i]->changePecaTheme(m2);
			}
		}else if(app==2){
			if(pecas[i]->getPlayerNumber()==1){
				pecas[i]->changePecaTheme(d1);
			}else{
				pecas[i]->changePecaTheme(d2);
			}
		}else if(app==3){
			if(pecas[i]->getPlayerNumber()==1){
				pecas[i]->changePecaTheme(a1);
			}else{
				pecas[i]->changePecaTheme(a2);
			}
		}
	}
}

void LightingScene::drawPrimitives(string id){
	Node* node=this->scene[id];

	glPushMatrix();
	if(node->getTransforms().size()!=0){
		for(unsigned int j=0;j<node->getTransforms().size();j++){
			node->getTransforms()[j]->apply();
		}
	}

	if(node->getAppID()!=""){
		appearances[node->getAppID()]->apply();
	}

	if(node->getPrimitives().size()!=0){
		for(unsigned int j=0;j<node->getPrimitives().size();j++){
			node->getPrimitives()[j]->draw();
		}
	}else{
		for(unsigned int j=0;j<node->getDescendents().size();j++){
			if(node->getAppID()!=""){
				appearances[node->getAppID()]->apply();
			}
			drawPrimitives(node->getDescendents()[j]);
		}
	}
	glPopMatrix();
}

void LightingScene::initDisplayLists(string id){
	Node* node=this->scene[id];

	for(unsigned int j=0;j<node->getDescendents().size();j++){
		initDisplayLists(node->getDescendents()[j]);
	}

	if(node->getDisplayList()){
		int num =glGenLists(1);
		glNewList(num, GL_COMPILE);
		drawPrimitives(node->getRootID());
		glEndList();
		node->setDisplayValue(num);
	}
}

void LightingScene::drawAllThings(string id){
	Node* node=this->scene[id];
	n=node;

	if(!node->getDisplayList()){
		glPushMatrix();
		if(node->getAnimID()!=""){
			Animation* animation = this->animations[node->getAnimID()];
			animation->apply();
		}
		if(node->getTransforms().size()!=0){
			for(unsigned int j=0;j<node->getTransforms().size();j++){
				node->getTransforms()[j]->apply();
			}
		}

		if(node->getAppID()!=""){
			appearances[node->getAppID()]->apply();
		}
		if(node->getPrimitives().size()!=0){
			for(unsigned int j=0;j<node->getPrimitives().size();j++){
				node->getPrimitives()[j]->draw();
			}
		}else{
			for(unsigned int j=0;j<node->getDescendents().size();j++){
				if(node->getAppID()!=""){
					appearances[node->getAppID()]->apply();
				}
				drawAllThings(node->getDescendents()[j]);
			}
		}
		glPopMatrix();
	}else{
		glCallList(node->getDisplayValue());
	}
}


void LightingScene::update(unsigned long sysTm){
	map<string,Node*>:: iterator it=scene.begin();
	while(it!=scene.end()){
		if(it->second->getAnimID() != ""){
			animation = this->animations[it->second->getAnimID()];
			animation->update(sysTm);
		}
		for(unsigned int i=0; i < it->second->getPrimitives().size();i++){
			//peca1->update(sysTm);
			it->second->getPrimitives()[i]->update(sysTm); // Para o movimento nos shaders
		}
		it++;
	}
}

LightingScene::~LightingScene() 
{
	for(unsigned int i=0;i<lights.size();i++){
		delete(lights[i]);
	}
}

void LightingScene::setLightState(int pos, int value){
	lightState[pos]=value;
}

int LightingScene::getLightState(int pos){
	return lightState[pos];
}