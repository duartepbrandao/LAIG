#ifndef PICKSCENE_H
#define PICKSCENE_H

#include "CGFscene.h"
#include "PickInterface.h"

class PickScene : public CGFscene
{
	friend PickInterface;
public:
	void init();
	void display();
	~PickScene();
private:
	CGFlight* light0;
	CGFobject* object;
	CGFappearance* materialAppearance;
};

#endif