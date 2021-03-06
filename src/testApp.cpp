#include "testApp.h"

//--------------------------------------------------------------
testApp::testApp(ofxArgs* args){
	this->args = args;
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);

	bGameNotConfigured = false;
	gconfig = new GameConfig();
	
	if( gconfig->exists() ) {
		ofLogVerbose() << "Config file found, loading it...";
		gconfig->loadGameConfig();
	} else {
		bGameNotConfigured = true;
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
	gconfig->setup();
	(bGameNotConfigured) ? gconfig->show() : gconfig->hide();
	currentActivity = gconfig;
		
	booth = new PhotoBooth( cameraDeviceId );
	booth->setup();
	(!bGameNotConfigured) ? booth->show() : booth->hide();
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