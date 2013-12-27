#include "TPinterface.h"

// buffer to be used to store the hits during picking
#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];

TPinterface::TPinterface()
{
	testVar=0;

	c1 = new Peca("Classic/classic.yaf", 1, "Classic/player1.jpg", "Classic/player1.jpg");
	c2 = new Peca("Classic/classic.yaf", 2, "Classic/player2.jpg", "Classic/player2.jpg");
	m1 = new Peca("Mario/KoopaTropa.obj","Mario/mario.yaf", 1, "Mario/KoopaTropa.jpg", "Mario/base_2.jpg");
	m2 = new Peca("Mario/goomba.obj","Mario/mario.yaf", 2, "Mario/goomba.jpg", "Mario/base.jpg");
	d1 = new Peca("DragonBall/dragonball.yaf", 1, "DragonBall/player1.jpg", "DragonBall/player1.jpg");
	d2 = new Peca("DragonBall/dragonball.yaf", 2, "DragonBall/player2.jpg", "DragonBall/player2.jpg");
	a1 = new Peca("AngryBirds/red.obj","AngryBirds/angrybirds.yaf", 1, "AngryBirds/red.jpg", "AngryBirds/base.jpg");
	a2 = new Peca("AngryBirds/pig.obj","AngryBirds/angrybirds.yaf", 2, "AngryBirds/pig.jpg", "AngryBirds/base_2.jpg");

	char *s="createBoard(12,_).\n";
	TwixtSocket::envia(s, strlen(s));
	char ans[9000];
	TwixtSocket::recebe(ans);
	strncpy(board, ans, 9000);
	strncpy(boardTemp, ans, 9000);
}

void TPinterface::initGUI()
{
	GLUI_Panel *lightPanel= addPanel("Lights",1);
	for(unsigned int i=0;i<((LightingScene*) scene)->lightState.size();i++){
		char name[30]="";
		std::string s = ((LightingScene*) scene)->lights[i]->getID();
		sprintf(name, "%s", s.c_str());
		addCheckboxToPanel(lightPanel,name,&(((LightingScene*) scene)->lightState[i]),0);
	}

	addColumn();

	GLUI_Panel *cameraPanel= addPanel("Cameras",1);
	if(((LightingScene*) scene)->cameras.size()<5){
		GLUI_RadioGroup *radioCameras=addRadioGroupToPanel(cameraPanel,&((LightingScene*) scene)->camera,1);
		for(unsigned int i=0;i<((LightingScene*) scene)->cameras.size();i++){
			char name[30]="";
			std::string s =((LightingScene*) scene)->cameras[i]->getID();
			sprintf(name, "%s", s.c_str());
			addRadioButtonToGroup(radioCameras,name);
		}
	}
	else{
		GLUI_Listbox *listbox = addListboxToPanel(cameraPanel,"Cameras",&((LightingScene*) scene)->camera,2);

		for(unsigned int i=0;i<2;i++){
			char name[30]="";
			std::string s =((LightingScene*) scene)->cameras[i]->getID();
			sprintf(name, "%s", s.c_str());
			listbox->add_item(i,name);
			((LightingScene*) scene)->camera= listbox->get_int_val();
		}
	}

	addColumn();

	GLUI_Panel *drawmodePanel= addPanel("Drawmode",1);
	GLUI_RadioGroup *radioDrawmode=addRadioGroupToPanel(drawmodePanel,&((LightingScene*) scene)->drawmode,3);
	addRadioButtonToGroup(radioDrawmode,"Point");
	addRadioButtonToGroup(radioDrawmode,"Line");
	addRadioButtonToGroup(radioDrawmode,"Fill");

	addColumn();

	GLUI_Panel *themesPanel= addPanel("Themes",1);
	GLUI_Listbox *themeslistbox = addListboxToPanel(themesPanel,"Themes",&((LightingScene*) scene)->app,4);
	themeslistbox->add_item(0, "classic");
	themeslistbox->add_item(1, "mario");
	themeslistbox->add_item(2, "dragon ball");
	themeslistbox->add_item(3, "angry birds");
}

void TPinterface::processGUI(GLUI_Control *ctrl)
{
	printf ("GUI control id: %d\n  ",ctrl->user_id);
	switch (ctrl->user_id)
	{
	case 0:
		for(unsigned int i=0;i<((LightingScene*) scene)->lightState.size();i++){
			printf ("New Light %d: %d\n",i,((LightingScene*) scene)->lightState[i]);
		}
		break;
	case 4:
		((LightingScene*) scene)->appChoose=false;
		break;
	};
}

void TPinterface::processMouse(int button, int state, int x, int y) 
{
	CGFinterface::processMouse(button,state, x, y);

	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y);
}

void TPinterface::performPicking(int x, int y) 
{
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer (BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);

	// Initialize the picking name stack
	glInitNames();

	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);

	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix ();

	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,projmat);

	// reset projection matrix
	glLoadIdentity();

	// get current viewport and use it as reference for 
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// this is multiplied in the projection matrix
	gluPickMatrix ((GLdouble) x, (GLdouble) (CGFapplication::height - y), 5.0, 5.0, viewport);

	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);

	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer
	scene->display();

	// restore original projection matrix
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();

	glFlush();

	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void TPinterface::processHits (GLint hits, GLuint buffer[]) 
{
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected=NULL;
	GLuint nselected;

	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			selected = ptr;
			nselected=num;
		}
		for (int j=0; j < num; j++) 
			ptr++;
	}

	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected!=NULL)
	{
		// this should be replaced by code handling the picked object's ID's (stored in "selected"), 
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");
		for (int i=0; i<nselected; i++){
			printf("%d ",selected[i]);
		}
		float deltaX=selected[0]*2.57*1.5+3.05+1.5/2;
		float deltaY=selected[1]*2.57*1.5+3.1+1.5/2;


		//Verifica se é possível jogar numa determinada posicao
		int num=1;
		char letra;
		if(!((LightingScene*) scene)->pecas.empty()){
			num=((LightingScene*) scene)->pecas[((LightingScene*) scene)->pecas.size()-1]->getPlayerNumber();
			if(num==2){
				num=1;
				letra='x';
			}else if(num==1){
				num=2;
				letra='y';
			}
		}else{
			num=1;
			letra='x';
		}

		char s[100];
		sprintf (s, "verify_place(%d, %d,_, _, %d, 12).\n", selected[0]+1, selected[1]+1, num);
		TwixtSocket::envia(s, strlen(s));
		char ans[128];
		TwixtSocket::recebe(ans);
		
		/*char s2[9000];
		sprintf (s2, "replaceMatrix(%s, %d, %d,%c,_, _, 0).\n", board, selected[0]+1, selected[1]+1,letra);
		TwixtSocket::envia(s2, strlen(s2));
		char ans2[9000];
		TwixtSocket::recebe(ans2);
		strncpy(board, ans2, 9000);*/
		
		if(ans[0]=='0'){
			if(num==1){
				loadPecasPlayer1(deltaX, deltaY);
			}else if(num==2){
				loadPecasPlayer2(deltaX, deltaY);
			}
		}else{
			printf("%s\n", ans);
		}
		//end

		printf("\n");
	}
	else
		printf("Nothing selected while picking \n");	
}

void TPinterface::loadPecasPlayer1(float deltaX, float deltaY){
	switch(((LightingScene*) scene)->app){
	case 0:
		((LightingScene*) scene)->pecas.push_back(new Peca(c1));
		break;
	case 1:
		((LightingScene*) scene)->pecas.push_back(new Peca(m1));
		break;
	case 2:
		((LightingScene*) scene)->pecas.push_back(new Peca(d1));
		break;
	case 3:
		((LightingScene*) scene)->pecas.push_back(new Peca(a1));
		break;
	}
	((LightingScene*) scene)->pecas[((LightingScene*) scene)->pecas.size()-1]->setPosx(deltaX);
	((LightingScene*) scene)->pecas[((LightingScene*) scene)->pecas.size()-1]->setPosy(deltaY);
}

void TPinterface::loadPecasPlayer2(float deltaX, float deltaY){

	switch(((LightingScene*) scene)->app){
	case 0:
		((LightingScene*) scene)->pecas.push_back(new Peca(c2));
		break;
	case 1:
		((LightingScene*) scene)->pecas.push_back(new Peca(m2));
		break;
	case 2:
		((LightingScene*) scene)->pecas.push_back(new Peca(d2));
		break;
	case 3:
		((LightingScene*) scene)->pecas.push_back(new Peca(a2));
		break;
	}
	((LightingScene*) scene)->pecas[((LightingScene*) scene)->pecas.size()-1]->setPosx(deltaX);
	((LightingScene*) scene)->pecas[((LightingScene*) scene)->pecas.size()-1]->setPosy(deltaY);
}

