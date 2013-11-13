#include <string>
#include <GL/glew.h>
#include <GL/glut.h>


class Globals{

	float background[4];
	int drawmode, shading, cullface, cullorder;
	bool doublesided, local, enabled, ambient;

public:
	Globals(float* background, std::string drawmode,std::string  shading, std::string cullface,std::string  cullorder);
	~Globals();
	int setValue(std::string s);
	float* getBackgroundPointer();
	float getBackground(int i);
	int getDrawMode();
	int getShading();
	int getCullface();
	int getCullorder();
};

