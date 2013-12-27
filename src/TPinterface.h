#ifndef TPinterface_H
#define TPinterface_H

#include "CGFinterface.h"
#include "LightingScene.h"


class TPinterface: public CGFinterface {
	public:

		int app;
		char board[9000];
		char boardTemp[9000];

		Peca *c1, *c2, *m1, *m2, *d1, *d2, *a1, *a2;

		TPinterface();

		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);

		virtual void processMouse(int button, int state, int x, int y);	
		void performPicking(int x, int y);
		void processHits(GLint hits, GLuint buffer[]); 

		int testVar;

		void loadPecasPlayer1(float deltaX, float deltaY);
		void loadPecasPlayer2(float deltaX, float deltaY);
};


#endif
