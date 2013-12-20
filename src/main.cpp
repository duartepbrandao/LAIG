#include <iostream>
#include <exception>
#include "XMLScene.h"
#include <iostream>
#include <exception>
#include "CGFapplication.h"
#include "LightingScene.h"
#include "TPinterface.h"
//#include "TwixtSocket.h"

using std::cout;
using std::exception;

int main(int argc, char* argv[]) {

	XMLScene xmlscene=XMLScene("Classic/classic.yaf"/*"teste.yaf"*/);

	//TwixtSocket* ts =new TwixtSocket("");

	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		app.setScene(new LightingScene(xmlscene.getLights(), xmlscene.getGlobals(), xmlscene.getCameras(), xmlscene.getRootID(), xmlscene.getScene(), xmlscene.getAppearances(), xmlscene.getAnimations()));
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