#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxArgs.h"
#include "photobooth.h"
#include "gameconfig.h"

class testApp : public ofBaseApp{
	public:
		testApp(ofxArgs *args);

		void setup();
		void update();
		void draw();
		
		void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxArgs* args;
		bool bGameNotConfigured;
		Activity *currentActivity;
		PhotoBooth *booth;
		GameConfig *gconfig;
};
