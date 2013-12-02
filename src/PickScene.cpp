#include "PickScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"

#include "ExampleObject.h"

#include <math.h>

#include "CGFappearance.h"

#define NUM_OBJS 7
#define NUM_ROWS 5
#define NUM_COLS 4

void PickScene::init() 
{
	// normal init, no changes needed

	// Enables lighting computations
	glEnable(GL_LIGHTING);

	// Sets up some lighting parameters
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient);  // Define ambient light
	
	// Declares and enables a light
	float light0_pos[4] = {4.0, 6.0, 5.0, 1.0};
	light0 = new CGFlight(GL_LIGHT0, light0_pos);
	light0->enable();

	// Defines a default normal
	glNormal3f(0,0,1);

	object=new ExampleObject();
	materialAppearance=new CGFappearance();
}

void PickScene::display() 
{

	// ---- BEGIN Background, camera and axis setup
	
	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();

	// Draw (and update) light
	light0->draw();

	// Draw axis
	axis.draw();


	// ---- END Background, camera and axis setup


	// ---- BEGIN feature demos

	materialAppearance->apply();

	// scale down a bit	
	glScalef(0.2, 0.2, 0.2);

	// picking example, the important parts are the gl*Name functions
	// and the code in the associted PickInterface class

	// Example 1: simple naming
	glPushMatrix();

	glPushName(-1);		// Load a default name

	for (int i=0; i< NUM_OBJS;i++)
	{
		glPushMatrix();
		glTranslatef(i*5,0,0);
		glLoadName(i);		//replaces the value on top of the name stack
		object->draw();
		glPopMatrix();
	}
	glPopMatrix();

	// example 2: structured naming
	for (int r=0; r < NUM_ROWS; r++)
	{
		glPushMatrix();
		glTranslatef(0, r*4, 0);
		glLoadName(r);
		for (int c=0; c < NUM_COLS; c++)
		{
			glPushMatrix();
			glTranslatef(0,0,(c+1)*5);
			glRotatef(90,0,1,0);
			glPushName(c);
			object->draw();
			glPopName();
			glPopMatrix();
		}
		glPopMatrix();
	}

	// ---- END feature demos

	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

PickScene::~PickScene()
{
	delete(materialAppearance);
	delete(object);
	delete(light0);
}
