#include "TwixtSocket.h"
#include "XMLScene.h"
#include "CGFapplication.h"
#include "TPinterface.h"

int main(int argc, char* argv[]) {

	XMLScene xmlscene=XMLScene("Classic/classic.yaf");
	LightingScene *ls=new LightingScene(xmlscene.getLights(), xmlscene.getGlobals(), xmlscene.getCameras(), xmlscene.getRootID(), xmlscene.getScene(), xmlscene.getAppearances(), xmlscene.getAnimations());
	ls->addthemes(new LightingScene(xmlscene.getLights(), xmlscene.getGlobals(), xmlscene.getCameras(), xmlscene.getRootID(), xmlscene.getScene(), xmlscene.getAppearances(), xmlscene.getAnimations()));
	XMLScene xmlscene_1=XMLScene("Mario/mario.yaf");
	ls->addthemes(new LightingScene(xmlscene_1.getLights(), xmlscene_1.getGlobals(), xmlscene_1.getCameras(), xmlscene_1.getRootID(), xmlscene_1.getScene(), xmlscene_1.getAppearances(), xmlscene_1.getAnimations()));
	XMLScene xmlscene_2=XMLScene("DragonBall/dragonball.yaf");
	ls->addthemes(new LightingScene(xmlscene_2.getLights(), xmlscene_2.getGlobals(), xmlscene_2.getCameras(), xmlscene_2.getRootID(), xmlscene_2.getScene(), xmlscene_2.getAppearances(), xmlscene_2.getAnimations()));
	XMLScene xmlscene_3=XMLScene("AngryBirds/angrybirds.yaf");
	ls->addthemes(new LightingScene(xmlscene_3.getLights(), xmlscene_3.getGlobals(), xmlscene_3.getCameras(), xmlscene_3.getRootID(), xmlscene_3.getScene(), xmlscene_3.getAppearances(), xmlscene_3.getAnimations()));

	TwixtSocket* ts=new TwixtSocket();

	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		app.setScene(ls);
		app.setInterface(new TPinterface());

		app.run();
	}
	catch(GLexception& ex) {
		cout << "Erro: " << ex.what();
		return -1;
	}
	catch(exception& ex) {
		cout << "Erro inesperado: " << ex.what();
		return -1;
	}
	return 0;
}