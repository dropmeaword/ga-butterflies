#include "testApp.h"

//--------------------------------------------------------------
testApp::testApp(ofxArgs* args){
	this->args = args;
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);

	bGameNotConfigured = true;
	
	ofFile file(ofToDataPath("game.xml"));
	if( file.exists() ) {
		//game.loadConfig( file );
		ofLogNotice() << "Game configuration file found, loading game..." ;
		bGameNotConfigured = false;
	} else {
		bGameNotConfigured = true;
		ofLogNotice() << "Game configuration file NOT found. I will take you to the configuration screen" ;
	}
	
	ofLog() << "binary revision $Revision: $";
//	config();
	
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	glEnable(GL_DEPTH_TEST);
	
	int cameraDeviceId = -1;
	if(this->args->contains("-device")){
		ofLogVerbose() << "camera device specified on command line.";
		cameraDeviceId = this->args->getInt("-device");
	}
	
	// create different activities
	gconfig = new GameConfig();
	gconfig->setup();
	(bGameNotConfigured) ? gconfig->show() : gconfig->hide();
	currentActivity = gconfig;
		
	booth = new PhotoBooth( cameraDeviceId );
	booth->setup();
	booth->hide();
	currentActivity = booth;
}

void testApp::exit() {
	delete gconfig;
	delete booth;
}

//--------------------------------------------------------------
void testApp::update(){
	if(bGameNotConfigured) {
		gconfig->update();
	} else {
		ofSetWindowTitle(ofToString(ofGetFrameRate()));	
		booth->update();
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	if(bGameNotConfigured) {
		gconfig->draw();
	} else {
		booth->draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

	switch(key) {
	
		case 's':
		case 'S':
			booth->videoSettings();
			break;
	
		case 'b':
		case 'B':
			booth->bSyncBackground = true;
			break;

		case 'm':
		case 'M':
			if(booth->cam.getMouseInputEnabled()) {
				booth->cam.disableMouseInput();
			} else {
				booth->cam.enableMouseInput();
			}
			break;

		case 'a':
		case 'A':
			// toggle animation of butterfly preview
			break;
	} // switch
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}