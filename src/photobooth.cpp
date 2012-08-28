#include "photobooth.h"

PhotoBooth::PhotoBooth(int camid) 
	: Activity("PhotoBooth") {
	cameraDeviceId = camid;
	bInitialized = false;

	bInvert = false;

	bDetectHoles = true;
	iBlobSizeMin = (FEED_WIDTH*FEED_HEIGHT)*.2;
	iBlobSizeMax = (FEED_WIDTH*FEED_HEIGHT)*.9;
	
	bFlapConstantly = false;
	
	eq.loadImage("eq.png");
	plus.loadImage("plus.png");

	// intialize UI elements
	int panelWidth = 500;
	gui = new ofxUICanvas(ofGetScreenWidth() - panelWidth, 0, panelWidth, ofGetScreenHeight());
	gui->addWidgetDown(new ofxUILabel("CAMERA CALIBRATION", OFX_UI_FONT_MEDIUM)); 
	gui->addWidgetDown(new ofxUISlider(200, 12, .0, 600, 300,"THRESHOLD")); 
	gui->addWidgetDown(new ofxUISlider(200, 12, iBlobSizeMin, 600, 300,"SIZE MIN")); 
	gui->addWidgetDown(new ofxUISlider(200, 12, iBlobSizeMax, 600, 300,"SIZE MAX"));
	gui->addWidgetDown(new ofxUIToggle(16, 16, bInvert, "INVERT BINARY IMG"));
	gui->addWidgetDown(new ofxUIToggle(16, 16, bDetectHoles, "DETECT HOLES"));
	gui->addWidgetDown(new ofxUISlider(200, 12, 0, 360, butterfly.baseLineAngle, "WING BASE ANGLE"));
	gui->addWidgetDown(new ofxUILabelButton(false, "LEARN BACKGROUND", OFX_UI_FONT_SMALL));
	
	gui->addSpacer(panelWidth*.95, 2);
	gui->addWidgetDown(new ofxUILabel("BUTTERFLY", OFX_UI_FONT_MEDIUM)); 
	gui->addWidgetDown(new ofxUIToggle(16, 16, bFlapConstantly, "FLAP CONSTANTLY"));
	gui->addWidgetDown(new ofxUILabelButton(false, "MAKE BUTTERFLY", OFX_UI_FONT_SMALL));
	gui->addWidgetRight(new ofxUILabelButton(false, "DONE", OFX_UI_FONT_SMALL));
	
	ofAddListener(gui->newGUIEvent, this, &PhotoBooth::guiEvent);
	gui->setVisible( true );
	
	ofLogVerbose() << "Trying to load GUI settings for " + this->name;
	// load last settings
	gui->loadSettings("GUI/param.photobooth.xml");
}

PhotoBooth::~PhotoBooth() {
	gui->saveSettings("GUI/param.photobooth.xml"); 
	delete gui;
}

void PhotoBooth::videoSettings() {
	feed.videoSettings();
}

void PhotoBooth::setup() {
	ofLogVerbose() << "opening cameraDeviceId="<< cameraDeviceId << " for video feed.";
	feed.setDeviceID(cameraDeviceId);
	feed.setVerbose(true);
	feed.initGrabber(FEED_WIDTH, FEED_HEIGHT);
	
	//videoInverted 	= new unsigned char[camWidth*camHeight*3];
	vidTexture.allocate(FEED_WIDTH, FEED_HEIGHT, GL_RGB);	

	colorImg.allocate(FEED_WIDTH, FEED_HEIGHT);
	grayImage.allocate(FEED_WIDTH, FEED_HEIGHT);
	grayBg.allocate(FEED_WIDTH, FEED_HEIGHT);
	grayDiff.allocate(FEED_WIDTH, FEED_HEIGHT);
	
	bInitialized = true;
	bSyncBackground = true;

	// load wing bitmaps and apply them to butterfly
	ofImage imgLeft; 
	ofImage imgRight;
	imgLeft.loadImage("wing01.png");
	imgRight.loadImage("wing05.png");
	butterfly.setWings(imgLeft, imgRight);
	
	cam.setDistance(10);
	cam.setPosition(0, 2.0, 10);
	//cam.setupPerspective(true, 60, 0, 200);
	//cam.setDrag(0.9);
}

void PhotoBooth::update() {
	feed.grabFrame();
	
	if( feed.isFrameNew() ) {
		
		colorImg.setFromPixels(feed.getPixels(), FEED_WIDTH, FEED_HEIGHT);
		grayImage = colorImg;
		
		if (bSyncBackground == true){
			ofLogVerbose() << "synching image background...";
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bSyncBackground = false;
			//grayBg.invert();
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		if(bInvert) {
			grayDiff.invert();
		}

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, iBlobSizeMin, iBlobSizeMax, 10, bDetectHoles);	// find holes
	}
	
//	if (feed.isFrameNew()){
//		//int totalPixels = camWidth*camHeight*3;
//		unsigned char * pixels = feed.getPixels();
////		for (int i = 0; i < totalPixels; i++){
////			videoInverted[i] = 255 - pixels[i];
////		}
////		vidTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
//		vidTexture.loadData(pixels, FEED_WIDTH, FEED_HEIGHT, GL_RGB);
//	}

	if(bFlapConstantly) {
		butterfly.update( ofGetElapsedTimef() );
	}
}

void PhotoBooth::draw() {
	if(bInitialized) {
		ofClear(60, 60, 60);
		//feed.draw(20,20);

		// draw the incoming, the grayscale, the bg and the thresholded difference
//		colorImg.draw(20, 20, FEED_WIDTH/2, FEED_HEIGHT/2);
		int margin = 4;
		float previewWidth = FEED_WIDTH/3;
		float previewHeight = FEED_HEIGHT/3;
		
		float xx = margin;
		float yy = margin;

		grayImage.draw(xx, yy, previewWidth, previewHeight);
		//yy += previewHeight + margin;
		xx += previewWidth + margin;
		plus.draw(xx, (yy + ((previewHeight/2) - 10)) );
		xx += 20 + margin;
		grayBg.draw(xx, yy, previewWidth, previewHeight);
		xx += previewWidth + margin;
		eq.draw(xx, (yy + ((previewHeight/2) - 10)) );
		xx += 20 + margin;
		//yy += previewHeight + margin;
		grayDiff.draw(xx, yy, previewWidth, previewHeight);
		yy += previewHeight + margin;

		drawBlobContours(xx, yy, 320, 240);

		/*
		butterfly.imgLeft.draw(0, 0);
		butterfly.imgRight.draw(64, 0);
		*/
		float vs = 460;
		xx = margin + (vs/2);
		yy = yy + (vs/2);
		drawButterflyPreview(xx, yy, vs);
	} // if init
} // draw()

void PhotoBooth::drawBlobContours(float _x, float _y, float _width, float _height) {
		// then draw the contours:
		ofFill();
		ofSetHexColor(0x333333);
		ofRect(_x, _y, _width, _height);
		ofSetHexColor(0xffffff);

		// we could draw the whole contour finder
		//contourFinder.draw(360,540);
		ofPushMatrix();
			ofTranslate(_x, _y);
			ofScale(.5, .5); /// @note this .5 factor depends on the ratio between the width of camera input and the width of the contour draw area

			// or, instead we can draw each blob individually,
			// this is how to get access to them:
			for (int i = 0; i < contourFinder.nBlobs; i++){
					contourFinder.blobs[i].draw(0, 0);
			} // for contours
		ofPopMatrix();
}

void PhotoBooth::drawButterflyPreview(float xx, float yy, float vwSize) {
	ofRectangle viewport = ofRectangle( (xx - (vwSize/2)), (yy - (vwSize/2)), vwSize, vwSize);
	
		ofPushMatrix();
			ofTranslate(xx, yy, 0);
			// draw viewer area
			ofFill();
			ofSetHexColor(0x333333);
			ofRect(-vwSize/2, -vwSize/2, vwSize, vwSize);
			ofSetHexColor(0xffffff);

			cam.begin( viewport );
			butterfly.draw();
			cam.end();
		ofPopMatrix();
}


void PhotoBooth::guiEvent(ofxUIEventArgs &e)
{
/*
	gui->addWidgetDown(new ofxUISlider(200, 12, .0, 600, 300,"SIZE MIN")); 
	gui->addWidgetDown(new ofxUISlider(200, 12, .0, 600, 300,"SIZE MAX"));
	gui->addWidgetDown(new ofxUIToggle(32, 32, false, "INVERT BINARY IMG"));
	gui->addWidgetDown(new ofxUIToggle(32, 32, false, "DETECT HOLES"));
*/
		if(e.widget->getName() == "THRESHOLD") {
        ofxUISlider *slider = ((ofxUISlider *) e.widget);
				threshold = (int)slider->getScaledValue();
				//ofLogNotice() << "slider value = " << threshold;
				//grass->paramNscale( k );
		} else if(e.widget->getName() == "INVERT BINARY IMG") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bInvert = toggle->getValue();
		} else if(e.widget->getName() == "SIZE MIN") {
        ofxUISlider *slider = ((ofxUISlider *) e.widget);
        iBlobSizeMin = (int)slider->getScaledValue();
		} else if(e.widget->getName() == "SIZE MAX") {
        ofxUISlider *slider = ((ofxUISlider *) e.widget);
        iBlobSizeMax = (int)slider->getScaledValue();
		} else if(e.widget->getName() == "WING BASE ANGLE") {
        ofxUISlider *slider = ((ofxUISlider *) e.widget);
        butterfly.baseLineAngle = (float)slider->getScaledValue();
		} else if(e.widget->getName() == "DETECT HOLES") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bDetectHoles = toggle->getValue();
		} else if(e.widget->getName() == "FLAP CONSTANTLY") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bFlapConstantly = toggle->getValue();
		}
} // handler
