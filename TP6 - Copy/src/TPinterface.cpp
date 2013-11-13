#include "TPinterface.h"
#include "LightingScene.h"

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
		addCheckboxToPanel(lightPanel,name,&(((LightingScene*) scene)->lightState[i]),NULL);
	}

	addColumn();

	GLUI_Panel *cameraPanel= addPanel("Cameras",1);
	if(((LightingScene*) scene)->cameras.size()<5){
		GLUI_RadioGroup *radioCameras=addRadioGroupToPanel(cameraPanel,&((LightingScene*) scene)->camera,NULL);
		for(unsigned int i=0;i<((LightingScene*) scene)->cameras.size();i++){
			char name[30]="";
			std::string s =((LightingScene*) scene)->cameras[i]->getID();
			sprintf(name, "%s", s.c_str());
			addRadioButtonToGroup(radioCameras,name);
		}
	}
	else{
		GLUI_Listbox *listbox = addListboxToPanel(cameraPanel,"Cameras",&((LightingScene*) scene)->camera,NULL);
		
		for(unsigned int i=0;i<((LightingScene*) scene)->cameras.size();i++){
			char name[30]="";
			std::string s =((LightingScene*) scene)->cameras[i]->getID();
			sprintf(name, "%s", s.c_str());
			listbox->add_item(i,name);
			((LightingScene*) scene)->camera= listbox->get_int_val();
		}
	}
	
	addColumn();

	GLUI_Panel *drawmodePanel= addPanel("Drawmode",1);
	GLUI_RadioGroup *radioDrawmode=addRadioGroupToPanel(drawmodePanel,&((LightingScene*) scene)->drawmode,NULL);
	addRadioButtonToGroup(radioDrawmode,"Point");
	addRadioButtonToGroup(radioDrawmode,"Line");
	addRadioButtonToGroup(radioDrawmode,"Fill");

	addColumn();
	
	GLUI_Panel* varPanel= addPanel("Animations", 1);
	addButtonToPanel(varPanel, "Reset", 3);

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
		case 3:
			((LightingScene*) scene)->animation->reset();
			break;
	};
}

