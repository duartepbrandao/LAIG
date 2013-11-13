#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "Primitive.h"
#include "Transforms.h"
#include "Appearence.h"


class Node
{
	string rootid, appID,animID;
	bool displayList;
	int displayValue;
	vector<Transforms*> transforms;
	vector<Primitive*> primitives;
	vector<string> descendents;
public:
	Node();
	~Node();
	void setRootId(string id);

	void addTransforms(Transforms* t);
	void addPrimitives(Primitive* p);
	void addDescendents(string d);
	string getAppID();
	void setAppID(string appID);

	vector<Transforms*> getTransforms();
	vector<Primitive*> getPrimitives();
	vector<string> getDescendents();
	string getRootID();

	//displayLists
	void setDisplayList(bool t);
	bool getDisplayList();
	void setDisplayValue(int v);
	int getDisplayValue();
	
	//Animations
	void setAnimID(std::string animID);
	string getAnimID();
};

#endif
