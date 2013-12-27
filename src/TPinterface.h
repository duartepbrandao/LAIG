#ifndef TPinterface_H
#define TPinterface_H

#include "CGFinterface.h"
#include "LightingScene.h"

class TPinterface: public CGFinterface {
	public:

		int app;

		Peca *c1, *c2, *m1, *m2, *d1, *d2, *a1, *a2;

		TPinterface();

		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);

		virtual void processMouse(int button, int state, int x, int y);	
		void performPicking(int x, int y);
		void performPicking2(int x, int y);
		virtual void processPassiveMouseMoved(int x, int y);
		void processHits(GLint hits, GLuint buffer[]); 
		void processHits2(GLint hits, GLuint buffer[]); 

		int testVar;

		void loadPecasPlayer1(float deltaX, float deltaY);
		void loadPecasPlayer2(float deltaX, float deltaY);
};


#endif
