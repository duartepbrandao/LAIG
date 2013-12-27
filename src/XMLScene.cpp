#include "XMLScene.h"

int XMLScene::num=0;

XMLScene::XMLScene(char *filename)
{
	num++;
	printf("Loading theme %d...\n",num);
	loadYAF(filename);
	if(num==4){
		printf("Load complete\n\n\n\n\n\n");
	}
}

void XMLScene::loadYAF(char* filename){
	// Read XML from file
	doc=new TiXmlDocument( filename );
	bool loadOkay = doc->LoadFile();

	//loadFile
	if ( !loadOkay ){
		printf( "Could not load file '%s'. Error='%s'. Exiting.\n", filename, doc->ErrorDesc() );
		exit( 1 );
	}

	TiXmlElement* yafElement= doc->FirstChildElement( "yaf" );
	if (yafElement == NULL)
	{
		printf("Main yaf block element not found! Exiting!\n");
		exit(1);
	}

	// Init Element
	initElement = yafElement->FirstChildElement( "globals" );
	doInitElement();

	// Camera Element
	cameraElement = yafElement->FirstChildElement("cameras");
	doCameraElement();

	// lighting Element
	lightingElement = yafElement->FirstChildElement("lighting");
	doLightingElement();

	// textures Element
	texturesElement = yafElement->FirstChildElement("textures");
	doTextureElement();

	// Appearance Element
	appearancesElement = yafElement->FirstChildElement("appearances");
	doAppearanceElement();

	//animations Element
	animationsElement = yafElement->FirstChildElement("animations");
	doAnimationElement();

	//Graph Element
	graphElement =  yafElement->FirstChildElement( "graph" );
	doGraphElement();
}


void XMLScene::doInitElement(){
	std::string drawmode, shading, cullface, cullorder;
	float background[4];
	if (initElement == NULL){
		printf("Init block not found!\n");
	}else{
		if (initElement){
			read4Float("background",initElement, background);
			drawmode=readString("drawmode",initElement);
			shading=readString("shading",initElement);
			cullface=readString("cullface",initElement);
			cullorder=readString("cullorder",initElement);

			globals=new Globals(background, drawmode, shading, cullface, cullorder);

		}else{
			printf("init not found\n");
		}
	}
}

void XMLScene::doCameraElement(){
	float pos[3], target[3];
	float near, far, angle, left, right, top, bottom;
	string initial;
	std::string id;
	if (cameraElement == NULL){
		printf("camera block not found!\n");
	}else{

		if (cameraElement){
			initial=readString("initial",cameraElement);

			//perspective camera
			if(perspectiveElement=cameraElement->FirstChildElement( "perspective" )){
				id=readString("id",perspectiveElement);
				near=readFloat("near",perspectiveElement);
				far=readFloat("far",perspectiveElement);
				angle=readFloat("angle",perspectiveElement);
				read3Float("pos",perspectiveElement, pos);
				read3Float("target",perspectiveElement, target);

				camera.push_back(new PerspectiveCamera(id, near, far, angle, pos, target,initial));

				while(perspectiveElement=perspectiveElement->NextSiblingElement("perspective")){
					id=readString("id",perspectiveElement);
					near=readFloat("near",perspectiveElement);
					far=readFloat("far",perspectiveElement);
					angle=readFloat("angle",perspectiveElement);
					read3Float("pos",perspectiveElement, pos);
					read3Float("target",perspectiveElement, target);

					camera.push_back(new PerspectiveCamera(id, near, far, angle, pos, target,initial));
				}
			}
			//ortho camera
			if((orthoElement=cameraElement->FirstChildElement( "ortho" ))){
				id=readString("id",orthoElement);
				near=readFloat("near",orthoElement);
				far=readFloat("far",orthoElement);
				left=readFloat("left",orthoElement);
				right=readFloat("right",orthoElement);
				top=readFloat("top",orthoElement);
				bottom=readFloat("bottom",orthoElement);

				camera.push_back(new OrthoCamera(id, near, far, left, right, top, bottom,initial));

				while(orthoElement=orthoElement->NextSiblingElement("ortho")){
					id=readString("id",orthoElement);
					near=readFloat("near",orthoElement);
					far=readFloat("far",orthoElement);
					left=readFloat("left",orthoElement);
					right=readFloat("right",orthoElement);
					top=readFloat("top",orthoElement);
					bottom=readFloat("bottom",orthoElement);

					camera.push_back(new OrthoCamera(id, near, far, left, right, top, bottom,initial));
				}
			}
		}
	}
}

void XMLScene::doLightingElement(){
	std::string id;
	unsigned int idLight=GL_LIGHT0,enabled,doublesided,local, enable;
	float position[4], ambient[4], diffuse[4], specular[4], gAmbient[4], direction[3], angle, exp;

	position[3]=1;
	if (lightingElement == NULL){
		printf("lighting block not found!\n");
	}else{

		if (lightingElement){

			doublesided=readBool("doublesided",lightingElement);
			local=readBool("local",lightingElement);
			enable=readBool("enabled",lightingElement);
			read4Float("ambient",lightingElement,gAmbient);

			//omni light
			omniElement=lightingElement->FirstChildElement( "omni" );
			if(omniElement!=NULL){
				while(omniElement){
					id=readString("id",omniElement);
					enabled=readBool("enabled",omniElement);
					read3Float("location",omniElement,position);
					read4Float("ambient",omniElement,ambient);
					read4Float("diffuse",omniElement,diffuse);
					read4Float("specular",omniElement,specular);

					lights.push_back(new Light(id,doublesided, local, enable, gAmbient, ambient, specular, diffuse, enabled, idLight, position, NULL));
					idLight++;

					omniElement=omniElement->NextSiblingElement("omni");
				}
			}

			//spot light
			spotElement=lightingElement->FirstChildElement( "spot" );
			if(spotElement!=NULL){
				while(spotElement){
					id=readString("id",spotElement);
					enabled=readBool("enabled",spotElement);
					read3Float("location",spotElement,position);
					read4Float("ambient",spotElement,ambient);
					read4Float("diffuse",spotElement,diffuse);
					read4Float("specular",spotElement,specular);
					angle=readFloat("angle",spotElement);
					exp=readFloat("exponent",spotElement);
					read3Float("direction",spotElement, direction);

					lights.push_back(new SpotLight(angle, exp, id,doublesided, local, enable, gAmbient, ambient, specular, diffuse, enabled, idLight, position, direction));
					idLight++;

					spotElement=spotElement->NextSiblingElement("spot");
				}
			}
		}
	}
}

void XMLScene::doTextureElement(){
	std::string id, file;
	if (texturesElement == NULL){
		printf("textures block not found!\n");
	}else{
		if(texturesElement){
			textureElement=texturesElement->FirstChildElement( "texture" );
			if(textureElement!=NULL){
				while(textureElement){
					id=readString("id",textureElement);
					file=readString("file",textureElement);

					Texture* t=new Texture(id, file);
					textures[id]=t;
					textureElement=textureElement->NextSiblingElement("texture");
				}
			}
		}
	}
}

void XMLScene::doAppearanceElement(){
	Appearence* a;
	std::string id, tref;
	float ts, tt, shininess;
	float emissive[4], ambient[4], diffuse[4], specular[4];
	if (appearancesElement == NULL){
		printf("Appearance block not found!\n");
	}else{
		if (appearancesElement){
			appearanceElement=appearancesElement->FirstChildElement( "appearance" );
			if(appearanceElement!=NULL){
				while(appearanceElement){
					id=readString("id",appearanceElement);
					read4Float("emissive",appearanceElement, emissive);
					read4Float("ambient",appearanceElement, ambient);
					read4Float("diffuse",appearanceElement, diffuse);
					read4Float("specular",appearanceElement, specular);
					shininess=readFloat("shininess",appearanceElement);
					if((char *) appearanceElement->Attribute("textureref")!=NULL){
						tref=readString("textureref",appearanceElement);
						ts=readFloat("texlength_s",appearanceElement);
						tt=readFloat("texlength_t",appearanceElement);

						Texture* t=textures[tref];
						a=new Appearence(id, emissive, ambient, diffuse, specular, shininess, t->getPath(), ts, tt);
					}else{
						a=new Appearence(id, emissive, ambient, diffuse, specular, shininess);
					}
					appearances[id]=a;
					appearanceElement=appearanceElement->NextSiblingElement("appearance");
				}
			}
		}
	}
}

void XMLScene::doAnimationElement()
{
	Animation* anim;
	std::string id, type;
	float span;
	vector<float> tempControl;
	if (animationsElement !=NULL){
		if (animationsElement){
			animationElement=animationsElement->FirstChildElement("animation");
			if(animationElement!=NULL){
				while (animationElement){
					id=readString("id",animationElement);
					span=readFloat("span",animationElement);
					type=readString("type",animationElement);

					if(type=="linear"){
						anim=new LinearAnimation(id, span);
					}

					controlElement=animationElement->FirstChildElement("controlpoint");
					if(controlElement!=NULL){
						while(controlElement){
							tempControl.clear();
							tempControl.push_back(readFloat("xx",controlElement));
							tempControl.push_back(readFloat("yy",controlElement));
							tempControl.push_back(readFloat("zz",controlElement));
							anim->addControlPoint(tempControl);
							controlElement=controlElement->NextSiblingElement("controlpoint");
						}
					}
					anim->calculate();
					animations[id]=anim;
					animationElement=animationElement->NextSiblingElement("animation");
				}
			}
		}
	}
}

void XMLScene::doGraphElement(){
	//node, noderef
	std::string nodeId, id, axis, appID,animID, heightmap,texturemap,fragmentshader,vertexshader;
	float to[3], factor[3], angle;
	//primitives values
	float xy1[2], xy2[2], base, top, height, slices, stacks, inner, outer, loops, radius, xyz1[3], xyz2[3], xyz3[3];
	bool displayList=false;
	//patch
	int order, partsU, partsV;
	string compute;
	char* filename;

	float patch,plane;

	if (graphElement == NULL){
		printf("Graph block not found!\n");
	}else{

		if(graphElement){

			rootid=readString("rootid",graphElement);

			nodeElement=graphElement->FirstChildElement( "node" );
			if(nodeElement!=NULL){
				while(nodeElement){

					node=new Node();
					nodeId=readString("id",nodeElement);
					node->setRootId(nodeId);
					//displayList
					displayList=readBool("displaylist", nodeElement);
					node->setDisplayList(displayList);

					transformsElement=nodeElement->FirstChildElement( "transforms" );
					if(transformsElement!=NULL){
						//transforms
						transformElement=transformsElement->FirstChildElement();
						while(transformElement){
							//translate
							if(transformElement->Attribute("to")!=NULL){
								read3Float("to",transformElement,to);
								node->addTransforms(new Translate(to[0], to[1], to[2]));
							}
							//scale
							if(transformElement->Attribute("factor")!=NULL){//nunca entra aqui
								read3Float("factor", transformElement,factor);
								node->addTransforms(new Scale(factor[0], factor[1], factor[2]));
							}
							//rotate
							if(transformElement->Attribute("axis")!=NULL){
								axis=readString("axis", transformElement);
							}
							if(transformElement->Attribute("angle")!=NULL){
								angle=readFloat("angle", transformElement);
								node->addTransforms(new Rotate(angle,axis));
							}

							transformElement=transformElement->NextSiblingElement();
						}
					}

					//appearance
					appearancerefElement=nodeElement->FirstChildElement( "appearanceref" );
					if(appearancerefElement!=NULL){
						while(appearancerefElement){
							appID=readString("id",appearancerefElement);
							node->setAppID(appID);
							appearancerefElement=nodeElement->NextSiblingElement("appearanceref");
						}
					}

					//animations
					animationrefElement=nodeElement->FirstChildElement("animationref");
					if(animationrefElement != NULL){
						while(animationrefElement){
							animID=readString("id",animationrefElement);
							node->setAnimID(animID);
							animationrefElement=nodeElement->NextSiblingElement("animationref");
						}
					}

					//childrens
					childrenElement=nodeElement->FirstChildElement( "children" );
					if(childrenElement!=NULL){
						//noderef
						noderefElement=childrenElement->FirstChildElement( "noderef" );
						if(noderefElement!=NULL){
							while(noderefElement){
								id= readString("id",noderefElement);
								node->addDescendents(id);
								noderefElement=noderefElement->NextSiblingElement("noderef");
							}
						}

						//waterline
						waterlineElement=childrenElement->FirstChildElement("waterline");
						if (waterlineElement!=NULL)
						{
							while(waterlineElement){
								heightmap=readString("heightmap",waterlineElement);
								texturemap=readString("texturemap",waterlineElement);
								fragmentshader=readString("fragmentshader",waterlineElement);
								vertexshader=readString("vertexshader",waterlineElement);
								//Pwaterline* a = new Pwaterline("","","textureDemo2.frag","textureDemo2.vert");
								node->addPrimitives(new Pwaterline(heightmap,texturemap,fragmentshader,vertexshader));
								//node->addPrimitives(a);
								waterlineElement=waterlineElement->NextSiblingElement("waterline");
							}
						}

						//object
						objectElement=childrenElement->FirstChildElement("object");
						if (objectElement!=NULL)
						{
							while(objectElement){
								string str = readString("filename",objectElement);
								filename = new char[str.length() + 1];
								strcpy(filename, str.c_str());
								// do stuff
								obj=new Obj::File();
								obj->Load(filename);
								//obj2=new Object(filename);
								node->addPrimitives(obj);
								objectElement=objectElement->NextSiblingElement("object");
							}
						}

						//plane
						planeElement=childrenElement->FirstChildElement("plane");
						if(planeElement!=NULL){
							while(planeElement){
								plane=readFloat("parts",planeElement);
								node->addPrimitives(new Pplane(plane));
								planeElement=planeElement->NextSiblingElement("plane");
							}
						}
						//patch
						patchElement=childrenElement->FirstChildElement("patch");
						if(patchElement!= NULL){
							while(patchElement){
								order=readFloat("order",patchElement);
								partsU=readFloat("partsU",patchElement);
								partsV=readFloat("partsV",patchElement);
								compute=readString("compute",patchElement);
								Ppatch* pp=new Ppatch(order, partsU, partsV, compute);

								int i=0;
								controlPointElement=patchElement->FirstChildElement("controlpoint");
								if (controlPointElement!=NULL){
									while (controlPointElement){
										pp->addElementToGrid(i, 0, readFloat("x",controlPointElement));
										pp->addElementToGrid(i, 1, readFloat("y",controlPointElement));
										pp->addElementToGrid(i, 2, readFloat("z",controlPointElement));
										i++;
										controlPointElement=controlPointElement->NextSiblingElement("controlpoint");
									}
								}
								node->addPrimitives(pp);
								patchElement=patchElement->NextSiblingElement("patch");
							}
						}

						//rectangle
						rectangleElement=childrenElement->FirstChildElement( "rectangle" );
						if(rectangleElement!=NULL){
							while(rectangleElement){
								read2Float("xy1",rectangleElement, xy1);
								read2Float("xy2",rectangleElement, xy2);
								node->addPrimitives(new Prectangle(xy1[0], xy1[1],xy2[0], xy2[1]));
								rectangleElement=rectangleElement->NextSiblingElement("rectangle");
							}
						}
						//cylinder
						cylinderElement=childrenElement->FirstChildElement( "cylinder" );
						if(cylinderElement!=NULL){
							while(cylinderElement){
								base=readFloat("base",cylinderElement);
								top=readFloat("top",cylinderElement);
								height=readFloat("height",cylinderElement);
								slices=readFloat("slices",cylinderElement);
								stacks=readFloat("stacks",cylinderElement);
								node->addPrimitives(new Pcylinder(base,top,height,slices,stacks));
								cylinderElement=cylinderElement->NextSiblingElement("cylinder");
							}
						}
						//torus
						torusElement=childrenElement->FirstChildElement( "torus" );
						if(torusElement!=NULL){
							while(torusElement){
								inner=readFloat("inner",torusElement);
								outer=readFloat("outer",torusElement);
								slices=readFloat("slices",torusElement);
								loops=readFloat("loops",torusElement);
								node->addPrimitives(new Ptorus(inner, outer, slices, loops));
								torusElement=torusElement->NextSiblingElement("torus");
							}
						}
						//sphere
						sphereElement=childrenElement->FirstChildElement( "sphere" );
						if(sphereElement!=NULL){
							while(sphereElement){
								radius=readFloat("radius",sphereElement);
								slices=readFloat("slices",sphereElement);
								stacks=readFloat("stacks",sphereElement);
								node->addPrimitives(new Psphere(radius, slices, stacks));
								sphereElement=sphereElement->NextSiblingElement("sphere");
							}
						}
						//triangle
						triangleElement=childrenElement->FirstChildElement( "triangle" );
						if(triangleElement!=NULL){
							while(triangleElement){
								read3Float("xyz1",triangleElement, xyz1);
								read3Float("xyz2",triangleElement, xyz2);
								read3Float("xyz3",triangleElement, xyz3);
								node->addPrimitives(new Ptriangle(xyz1[0], xyz1[1],xyz1[2],xyz2[0],xyz2[1],xyz2[2],xyz3[0],xyz3[1],xyz3[2]));
								triangleElement=triangleElement->NextSiblingElement("triangle");
							}
						}
					}
					scene[nodeId]=node;
					nodeElement=nodeElement->NextSiblingElement("node");
				}
			}
		}
	}
}

vector<Light*> XMLScene::getLights(){
	return lights;
}

Globals* XMLScene::getGlobals(){
	return globals;
}

vector<Camera*> XMLScene::getCameras(){
	return camera;
}

string XMLScene::getRootID(){
	return rootid;
}

map<string,Texture*> XMLScene::getTextures(){
	return textures;
}

map<string, Node*> XMLScene::getScene(){
	return scene;
}

map<string, Appearence*> XMLScene::getAppearances(){
	return appearances;
}

std::string XMLScene::readString(char* attribute, TiXmlElement* element){
	char *valString=NULL;
	valString=(char *) element->Attribute(attribute);
	//std::string stg;
	char stg[100];

	if(valString && sscanf(valString,"%s",stg)==1){
		//printf("%s: %s\n",attribute,stg );
	}else{
		//printf("%s attribute not found!\n", attribute);
	}
	return std::string(stg);
}

int XMLScene::readBool(char* attribute, TiXmlElement* element){
	std::string stg = readString(attribute, element);

	if(stg.compare("true")==0){
		return 1;
	}
	return 0;
}

float XMLScene::readFloat(char* attribute,TiXmlElement* element){
	char *valString=NULL;
	float f1=0;

	valString=(char *) element->Attribute(attribute);

	if(valString && sscanf(valString,"%f",&f1)==1){
		//printf("%s values: %f\n", attribute, f1);
	}else{
		//printf("%s attribute not found!\n", attribute);
	}
	return f1;
}

void XMLScene::read2Float(char* attribute,TiXmlElement* element, float* flt){
	char *valString=NULL;

	valString=(char *) element->Attribute(attribute);

	if(valString && sscanf(valString,"%f %f",&flt[0], &flt[1])==2){
		//printf("%s values: %f %f\n", attribute, flt[0], flt[1]);
	}else{
		//printf("%s attribute not found!\n", attribute);
	}
}

void XMLScene::read3Float(char* attribute,TiXmlElement* element, float* flt){
	char *valString=NULL;

	valString=(char *) element->Attribute(attribute);

	if(valString && sscanf(valString,"%f %f %f",&flt[0], &flt[1], &flt[2])==3){
		//printf("%s values: %f %f %f\n", attribute, flt[0], flt[1], flt[2]);
	}else{
		//printf("%s attribute not found!\n", attribute);
	}
}

void XMLScene::read4Float(char* attribute,TiXmlElement* element, float* flt){
	char *valString=NULL;

	valString=(char *) element->Attribute(attribute);

	if(valString && sscanf(valString,"%f %f %f %f",&flt[0], &flt[1], &flt[2], &flt[3])==4){
		//printf("%s values: %f %f %f %f\n", attribute, flt[0], flt[1], flt[2], flt[3]);
	}else{
		//printf("%s attribute not found!\n", attribute);
	}
}

XMLScene::~XMLScene()
{
	delete(doc);
}

//-------------------------------------------------------

TiXmlElement *XMLScene::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
	// Searches within descendants of a parent for a node that has an attribute _attr_ (e.g. an id) with the value _val_
	// A more elaborate version of this would rely on XPath expressions
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}

map<string,Animation*> XMLScene::getAnimations()
{
	return animations;
}
