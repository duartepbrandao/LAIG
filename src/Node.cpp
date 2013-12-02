#include "Node.h"


Node::Node()
{
	appID="";
	animID="";
}


Node::~Node()
{
}

void Node::setRootId(string id){
	rootid=id;
}
	
void Node::addTransforms(Transforms* t){
	transforms.push_back(t);
}
	
void Node::addPrimitives(Primitive* p){
	primitives.push_back(p);
}
	
void Node::addDescendents(string d){
	descendents.push_back(d);
}

void Node::setAppID(string appID){
	this->appID=appID;
}

vector<Transforms*> Node::getTransforms(){
	return transforms;
}
	
vector<Primitive*> Node::getPrimitives(){
	return primitives;
}
	
vector<string> Node::getDescendents(){
	return descendents;
}
	
string Node::getAppID(){
	return appID;
}

string Node::getRootID(){
	return rootid;
}

void Node::setDisplayList(bool t){
	displayList=t;
}

bool Node::getDisplayList(){
	return displayList;
}

void Node::setAnimID(string animID )
{
	this->animID=animID;
}

string Node::getAnimID()
{
	return animID;
}
	
void Node::setDisplayValue(int v){
	displayValue=v;
}
	
int Node::getDisplayValue(){
	return displayValue;
}
