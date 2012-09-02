/*
 *  utils.cpp
 *  butterflies_ocv
 *
 *  Created by zilog on 8/28/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "utils.h"

namespace utils {

	void makeTransparent(ofImage &img) {
		img.setImageType(OF_IMAGE_COLOR_ALPHA); //make sure it can have alphas
		for(int i = 3; i < img.height*img.width*4; i+=4){
			if(img.getPixels()[i-1]==0) { //if blue is 0 make pixel transparent
				img.getPixels()[i]=0;
			}
		}
		img.update();
	}

	int countBlackPixels(ofImage &img) {
		int retval = 0;
		
		for(int i = 0; i < img.height*img.width*3; i+=3){
			if(img.getPixels()[i]==0) {
				retval++;
			}
		}
		
		return retval;
	}

}; // ns