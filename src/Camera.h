#include <CGFcamera.h>
#include <string>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

class Camera:public CGFcamera
{	static std::string initial;
	std::string id;
	float near, far;

public:
	Camera(std::string id, float near, float far,std::string initial);
	~Camera(void);
	std::string getID();
	std::string getInicial();
	float getNear();
	float getFar();
};

class OrthoCamera:public Camera{
	float left,  right,  top ,  bottom;
public:
	OrthoCamera(std::string id, float near, float far, float left, float right, float top , float bottom, std::string initial);
	float getLeft();
	float getRight();
	float getTop();
	void updateProjectionMatrix(int width, int height);
	float getBottom();
	void applyView();
};

class PerspectiveCamera: public Camera{

	float angle;
	float pos[3], target[3];

public:
	PerspectiveCamera(std::string id, float near, float far, float angle, float* pos, float* target, std::string initial);
	float getAngle();
	void updateProjectionMatrix(int width, int height);
	float* getPos();
	float* getTarget();
	void applyView();
};