#ifndef __PHOTOBOOTH_H__
#define __PHOTOBOOTH_H__

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxUI.h"
#include "activity.h"
#include "butterfly.h"

#define FEED_WIDTH 640 //320
#define FEED_HEIGHT 480 //240


class PhotoBooth : public Activity {
public:
	PhotoBooth(int camid);
	virtual ~PhotoBooth();

	void videoSettings();
	void setup();
	void update();
	void draw();
	
	void guiEvent(ofxUIEventArgs &e);

	void drawButterflyPreview(float xx, float yy, float vwSize);
	void drawBlobContours(float _x, float _y, float _width, float _height);
	
	ofxUICanvas *gui; 

	ofVideoGrabber 		feed;
	ofTexture		vidTexture;
	int cameraDeviceId;

	ofImage eq, plus;
	
	Butterfly butterfly;
	ofEasyCam cam; // for butterfly preview
	
	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	ofxCvContourFinder 	contourFinder;

	int threshold;
	bool bInvert;
	bool bInitialized;
	bool bSyncBackground;
	
	bool bFlapConstantly;
	
	bool bDetectHoles;
	int iBlobSizeMin, iBlobSizeMax;
};

#endif // __PHOTOBOOTH_H__