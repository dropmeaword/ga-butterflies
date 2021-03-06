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
	bMakingButterfly = false;
	
	bMakeButtonState = false;
	bLastMakeButtonState = false;
	
	debounceDelay = 50;  // ms
	
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

//	imgLeft.loadImage("placeholder.png");
//	imgRight.loadImage("placeholder.png");

	imgLeft.allocate(FEED_WIDTH, FEED_HEIGHT);
	imgRight.allocate(FEED_WIDTH, FEED_HEIGHT);

	lw.allocate(TEX_SIZE, TEX_SIZE);
	rw.allocate(TEX_SIZE, TEX_SIZE);

	// load wing bitmaps and apply them to butterfly
	ofImage wingLeft; 
	ofImage wingRight;
	wingLeft.loadImage("wing01.png");
	wingRight.loadImage("wing05.png");
	//butterfly.setWings(wingLeft, wingRight);
	
	cam.setDistance(10);
	cam.setPosition(1.5, 5.0, 10);
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

void PhotoBooth::show() {
	ofAddListener(gui->newGUIEvent, this, &PhotoBooth::guiEvent);
	//ofRegisterKeyEvents(this);
	ofAddListener(ofEvents().keyPressed, this, &PhotoBooth::keyPressed);
	gui->setVisible( true );
	Activity::show();
}

void PhotoBooth::hide() {
	ofRemoveListener(gui->newGUIEvent, this, &PhotoBooth::guiEvent);
	ofRemoveListener(ofEvents().keyPressed, this, &PhotoBooth::keyPressed);
	//ofUnregisterKeyEvents(this);
	gui->setVisible( false );
	Activity::hide();
}

void PhotoBooth::draw() {
	if(bInitialized) {
		ofClear(60, 60, 60);
		//feed.draw(20,20);

		// draw the incoming, the grayscale, the bg and the thresholded difference
//		colorImg.draw(20, 20, FEED_WIDTH/2, FEED_HEIGHT/2);
		int margin = 4;
		float tmp = .0;
		float previewWidth = FEED_WIDTH/3;
		float previewHeight = FEED_HEIGHT/3;
		
		float xx = margin;
		float yy = margin;

		grayImage.draw(xx, yy, previewWidth, previewHeight);
		//yy += previewHeight + margin;
		// draw line across the middle for alignment
		tmp = (xx + (previewWidth/2));
		ofSetColor(255, 255, 0);
		ofLine(tmp, yy, tmp, yy+previewHeight);
		ofSetColor(255, 255, 255);
		xx += previewWidth + margin;
		
		plus.draw(xx, (yy + ((previewHeight/2) - 10)) );
		xx += 20 + margin;

		grayBg.draw(xx, yy, previewWidth, previewHeight);
		xx += previewWidth + margin;

		eq.draw(xx, (yy + ((previewHeight/2) - 10)) );
		xx += 20 + margin;
		//yy += previewHeight + margin;

		grayDiff.draw(xx, yy, previewWidth, previewHeight);
		// draw line across the middle for alignment
		tmp = (xx + (previewWidth/2));
		ofSetColor(255, 255, 0);
		ofLine(tmp, yy, tmp, yy+previewHeight);
		ofSetColor(255, 255, 255);
		yy += previewHeight + margin;

		drawBlobContours(xx, yy, 320, 240);

		ofSetColor(255, 255, 255);
		imgLeft.draw(xx, yy+260, 128, 128);
		imgRight.draw(xx+128+margin, yy+260, 128, 128);

		lw.draw(xx, yy+260+140, 128, 128);
		rw.draw(xx+128+margin, yy+260+140, 128, 128);

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

void PhotoBooth::makeButterfly() {
	bMakingButterfly = true;
	ofLogNotice() << "making butterfly..." ;
	
	// get the bounding box of the butterfly
	int nblobs = contourFinder.blobs.size();
	// butterfly should be seen by the camera as a single blob if there are no blobs, there probably isn't a butterfly
	if(nblobs > 0) {
		ofLogNotice() << "making butterfly: blob found!" ;
		
		ofRectangle bbox = contourFinder.blobs[0].boundingRect;
		ofPolyline pline = ofPolyline( contourFinder.blobs[0].pts );

		ofLogNotice() << "blob.bbox [" << bbox.x << ", " << bbox.y << ", " << bbox.width << ", " << bbox.height << "]" ;
	
		int dimx = bbox.width/2;
		int dimy = bbox.height;
		imgLeft.allocate(dimx, dimy);
		imgRight.allocate(dimx, dimy);
		ofLogNotice() << "capturing wings with dimensions (" << dimx << ", " << dimy << ")" ;

		ofPoint *psrc = new ofPoint[4];
		psrc[0].set(bbox.x, bbox.y);
		psrc[1].set((bbox.x + bbox.width/2), bbox.y);
		psrc[2].set((bbox.x + bbox.width/2), bbox.y+bbox.height);
		psrc[3].set(bbox.x, bbox.y+bbox.height);

		ofPoint *pdst = new ofPoint[4];
		pdst[0].set(0, 0);
		pdst[1].set(imgLeft.width, 0);
		pdst[2].set(imgLeft.width, imgLeft.height);
		pdst[3].set(0, imgLeft.height);
		
		// make a rect for each wing
		// capture the contents of each rect in the grayDiff image
		//grayDiff.setROI(bbox.x, bbox.y, bbox.width/2, bbox.height);
//		imgLeft.allocate(FEED_WIDTH, FEED_HEIGHT, OF_IMAGE_COLOR_ALPHA);
		imgLeft.warpIntoMe(grayDiff, psrc, pdst);
//		imgLeft.allocate(bbox.width/2, bbox.height, OF_IMAGE_COLOR_ALPHA);
//		utils::copyRoiToImage(grayDiff, imgLeft);

		psrc[0].set((bbox.x + bbox.width/2), bbox.y);
		psrc[1].set((bbox.x + bbox.width), bbox.y);
		psrc[2].set((bbox.x + bbox.width), bbox.y+bbox.height);
		psrc[3].set((bbox.x + bbox.width/2), bbox.y+bbox.height);

		//grayDiff.setROI( (bbox.x+bbox.width/2), bbox.y, bbox.width/2, bbox.height);
		imgRight.warpIntoMe(grayDiff, psrc, pdst);
//		imgRight.allocate(FEED_WIDTH, FEED_HEIGHT, OF_IMAGE_COLOR_ALPHA);
//		imgLeft.allocate(bbox.width/2, bbox.height, OF_IMAGE_COLOR_ALPHA);
//		utils::copyRoiToImage(grayDiff, imgRight);
	}
	
	// we first have to convert the warped image to an ofImage so that we can 
	// mask-out the black pixels using the makeTransparent method
	ofImage tmp1, tmp2;
	tmp1.setFromPixels(imgLeft.getPixelsRef());
	utils::makeTransparent(tmp1);
	lw.begin();
	ofClear(0, 0, 0);
	ofEnableAlphaBlending();
	tmp1.draw(0, 0, TEX_SIZE, TEX_SIZE);
	ofDisableAlphaBlending();
	lw.end();

	tmp2.setFromPixels(imgRight.getPixelsRef());
	utils::makeTransparent(tmp2);
	rw.begin();
	ofClear(0, 0, 0);
	ofEnableAlphaBlending();
	tmp2.draw(0, 0, TEX_SIZE, TEX_SIZE);
	ofDisableAlphaBlending();
	rw.end();
	
	butterfly.setWings(lw, rw);
	bMakingButterfly = false;
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


void PhotoBooth::keyPressed(ofKeyEventArgs& eventArgs) {
	int key = eventArgs.key;

	switch(key) {
		case 's':
		case 'S':
			videoSettings();
			break;
	
		case 'b':
		case 'B':
			bSyncBackground = true;
			break;

		case 'm':
		case 'M':
			if(cam.getMouseInputEnabled()) {
				cam.disableMouseInput();
			} else {
				cam.enableMouseInput();
			}
			break;

		case 'a':
		case 'A':
			// toggle animation of butterfly preview
			break;
	} // switch
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
		} else if (e.widget->getName() == "MAKE BUTTERFLY") {
			ofLogNotice() << "Make butterfly button pressed..." ;
			ofxUILabelButton *button = (ofxUILabelButton *) e.widget; 
			int reading = button->getValue();
			
			if( (reading == true) && (bMakingButterfly != true) ) {
				makeButterfly();
			}
			
			/*
			if(reading != bLastMakeButtonState) {
				lastDebounceTime = ofGetElapsedTimeMillis();
			}
			
			if ((ofGetElapsedTimeMillis() - lastDebounceTime) > debounceDelay) {
				// whatever the reading is at, it's been there for longer
				// than the debounce delay, so take it as the actual current state:
				bMakeButtonState = reading;
			}
			
			if( (bMakeButtonState == true) && (bMakingButterfly != true) ) {
				makeButterfly();
			}
			
			bLastMakeButtonState = reading;
			*/
		}
		
} // handler
