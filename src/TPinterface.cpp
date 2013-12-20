#include "TPinterface.h"
#include "PickScene.h"

// buffer to be used to store the hits during picking
#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];

TPinterface::TPinterface()
{
	testVar=0;
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
		((LightingScene*) scene)->deltaX=selected[0]*2.57*1.5+3.05+1.5/2;
		((LightingScene*) scene)->deltaY=selected[1]*2.57*1.5+3.1+1.5/2;
		printf("\n");
	}
	else
		printf("Nothing selected while picking \n");	
}

