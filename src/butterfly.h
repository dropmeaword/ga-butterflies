#ifndef __BUTTERFLY_H__
#define __BUTTERFLY_H__

#include "ofMain.h"


class Butterfly {
public:
	ofVec3f pos;
	ofVec3f dir;
	
	bool bIsTextured;
	ofImage imgLeft, imgRight;
	
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

	void setWings(ofImage &l, ofImage &r);
	void clearWings();
	
	void update(float dt);
	void draw();
}; // class

#endif // __BUTTERFLY_H__
