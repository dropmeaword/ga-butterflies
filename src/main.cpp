#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(int argc, char *argv[]){
	ofxArgs* args = new ofxArgs(argc, argv);

	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1280, 960, OF_WINDOW);
	ofRunApp( new testApp( args )); // start the app
}
