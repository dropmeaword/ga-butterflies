/*
 *  utils.h
 *  butterflies_ocv
 *
 *  Created by zilog on 8/28/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#include "ofMain.h"
#include "ofxOpenCv.h"

namespace utils {
	void makeTransparent(ofImage &img);
	int countBlackPixels(ofImage &img);
//	void copyRoiToImage(ofxCvGrayscaleImage &src, ofImage &dst);
	void convert(ofxCvGrayscaleImage &src, ofImage &dst);
}; // ns