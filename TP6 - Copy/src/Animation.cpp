#include "Animation.h"

Animation::Animation(std::string id, float span){
	this-> id=id;
	this->span=span;
}

std::string Animation::getId(){
	return this->id;
}

void Animation::setId(std::string newId){
	this->id=newId;
}

float Animation::getSpan(){
	return span;
}

void Animation::setSpan(float span){
	this->span=span;
}
	
bool Animation::getDoReset(){
	return doReset;
}
	
void Animation::setDoReset(bool r){
	doReset=r;
}


LinearAnimation::LinearAnimation(std::string id,float span):Animation(id, span){
	counter=0;
}

void LinearAnimation::addControlPoint(std::vector<float> points){
	controlPoints.push_back(points);
}
	
std::vector<std::vector<float>> LinearAnimation::getControlPoints(){
	return controlPoints;
}

float LinearAnimation::getRot(){
	return rot;
}
	
void LinearAnimation::setRot(float rot){
	this->rot=rot;
}

void LinearAnimation::update(unsigned long sysTm){
	if (getDoReset()){
		calculate();
	}else{
		if(counter < controlPoints.size()-1){
			if((transX >= controlPoints[counter+1][0]-0.1 && transX <= controlPoints[counter+1][0]+0.1) && (transY >= controlPoints[counter+1][1]-0.1 && transY <= controlPoints[counter+1][1]+0.1) && (transZ >= controlPoints[counter+1][2]-0.1) && (transZ <= controlPoints[counter+1][2]+0.1)){
				counter++;
				if(counter < controlPoints.size()-1){
					float hip=sqrt (pow(controlPoints[counter+1][0]-controlPoints[counter][0],2)+pow(controlPoints[counter+1][2]-controlPoints[counter][2],2));
					
					if(controlPoints[counter+1][0]-controlPoints[counter][0]>=0){
						angle=acos((controlPoints[counter+1][2]-controlPoints[counter][2])/hip);
					}else{
						angle=-acos((controlPoints[counter+1][2]-controlPoints[counter][2])/hip);
					}
					if(hip!=0){
						rot=(180*angle)/(acos(-1.0));
					}else{
						rot=0;
					}

					deltaX = (controlPoints[counter+1][0]-controlPoints[counter][0])/distance;
					deltaY = (controlPoints[counter+1][1]-controlPoints[counter][1])/distance;
					deltaZ = (controlPoints[counter+1][2]-controlPoints[counter][2])/distance;

				}
			}else{
				transX += deltaX;
				transY += deltaY;
				transZ += deltaZ;
			}
		}
	}
}

void LinearAnimation::apply(){
	glTranslatef(transX, transY, transZ);
	glRotatef(rot,0,1,0);
}

void LinearAnimation::reset(){
	setDoReset(true);
}

void LinearAnimation::calculate(){
	counter=0;
	float d=0;
	float angulo,produto12,modulo1, modulo2, x,z;

	//Calcula a distancia total
	for(int i=0; i < controlPoints.size()-1; i++){
		d += sqrt(pow(controlPoints[i+1][0] - controlPoints[i][0],2) + pow(controlPoints[i+1][1] - controlPoints[i][1],2) + pow(controlPoints[i+1][2] - controlPoints[i][2],2));
	}
	distance=d;

	//Distâncias parciais que variam de acordo com a animação
	deltaX = (controlPoints[1][0]-controlPoints[0][0])/distance;
	deltaY = (controlPoints[1][1]-controlPoints[0][1])/distance;
	deltaZ = (controlPoints[1][2]-controlPoints[0][2])/distance;

	//transX, transY e tranZ contem os valores da primeira posicao na animação
	transX = deltaX;
	transY = deltaY;
	transZ = deltaZ;
	
	//rot a rotação no inicio é inexistente
	rot=angle=0;
	float hip=sqrt (pow(controlPoints[1][0]-controlPoints[0][0],2)+pow(controlPoints[1][2]-controlPoints[0][2],2));
	if(controlPoints[1][0]-controlPoints[0][0]>=0){
		angle=acos((controlPoints[1][2]-controlPoints[0][2])/hip);
	}else{
		angle=-acos((controlPoints[1][2]-controlPoints[0][2])/hip);
	}
	if(hip!=0){
		rot=(180*angle)/(acos(-1.0));
	}else{
		rot=0;
	}

	velocity = distance/getSpan();
	lastTime = 0;
	setDoReset(false);
}