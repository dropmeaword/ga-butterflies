#ifndef __BUTTERFLY_H__
#define __BUTTERFLY_H__

#include "ofMain.h"
#include "ofxOpenCv.h"

class Phenotype {
public:
	int iPixelsBlack, iPixelsWhite;
	float bwRatio;
//	ofImage imgLeft, imgRight;
	ofFbo imgLeft, imgRight;
	float fAverageOutlineSmoothness; /** this value is derived from capturing the 
																			 outline of the butterfly and calculating 
																			 the average angle between the normals of 
																			 the surrounding polyline */
	
	void save(string fname);
	void load(string fname);
}; // class


class Butterfly : public Phenotype {
public:
	ofVec3f pos;
	ofVec3f dir;
	
	bool bIsTextured;
	
	float speed;
	float phaseShift;
	float wingspan;
	float wingwidth;
	
	float flappingAngleRad;
	float wiggle;
	float beatingSpeed;
	
	float baseLineAngle;
	
	Butterfly();
	virtual ~Butterfly();

//	void setWings(ofImage &l, ofImage &r);
	void setWings(ofFbo &lt, ofFbo &rt);
	void clearWings();
	
	void update(float dt);
	void draw();
}; // class

#endif // __BUTTERFLY_H__
