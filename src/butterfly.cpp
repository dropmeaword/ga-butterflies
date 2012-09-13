#include "butterfly.h"
#include "utils.h"

void Phenotype::save(string fname) {
}

void Phenotype::load(string fname) {
}

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
Butterfly::Butterfly() {
	pos.set(0 ,0, 0);
	dir.set(0 ,0, 0);
	
	wingspan = 8.0;
	wingwidth = (wingspan / 2.0);

	speed = ofRandom(10) - 5.0;
	phaseShift = ofDegToRad( ofRandom(360) - 180);
	
	beatingSpeed = 1200;
	baseLineAngle = 120;
	
	bIsTextured = false;
	
	ofDisableArbTex();
	glEnable( GL_TEXTURE_2D );
//	// when texture area is small, bilinear filter the closest mipmap
//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//									 GL_LINEAR_MIPMAP_NEAREST );
//	// when texture area is large, bilinear filter the original
//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//
	// the texture wraps over at the edges (repeat)
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

//	imgLeft.loadImage("dot.png");
//	imgRight.loadImage("dot.png");
//	imgLeft.loadImage("wing.png");
//	imgRight.loadImage("wing.png");
}

Butterfly::~Butterfly() {
}

void Butterfly::setWings(ofFbo &lt, ofFbo &rt) {
//	imgLeft.clone(l);
//	imgRight.clone(r);

	imgLeft = lt;
	imgRight = rt;

	ofLogVerbose() << "Bufferfly::setWings (imgLeft.width) = " << imgLeft.getWidth() << " (imgLeft.height) = " << imgLeft.getHeight();
	
/*	
	utils::makeTransparent(imgLeft);
	utils::makeTransparent(imgRight);
*/
	bIsTextured = true;
}
/*
void Butterfly::setWings(ofImage &l, ofImage &r) {
	imgLeft.clone(l);
	imgRight.clone(r);
	
	utils::makeTransparent(imgLeft);
	utils::makeTransparent(imgRight);

	bIsTextured = true;
}
*/
void Butterfly::clearWings() {
//	imgLeft.clear();
//	imgRight.clear();
	bIsTextured = false;
}
	
void Butterfly::update(float t) {
	//flappingAngleRad = sin(ofDegToRad(baseLineAngle));
	//flappingAngleRad = (sin(ofDegToRad(t*500) + phaseShift)) / 4;
	float k = ofDegToRad(t*beatingSpeed);
	flappingAngleRad = 45 + ( sin(k) * 43 );
	//wiggle = sinf(k) * (wingwidth / 2);
	wiggle = sinf(k) / 3.0;
	//ofLogNotice() << "flappingAngleRad = " << flappingAngleRad;
}

void Butterfly::draw() {

	const GLfloat col[3] = {1.00,.451,.094};
	
	ofVec3f a = ofVec3f((-wingspan/2.0f), 0, wingwidth/2.0f);
	ofVec3f b = ofVec3f((-wingspan/2.0f), 0, -wingwidth/2.0f);
	ofVec3f c = ofVec3f(0, wiggle, (-wingwidth/2.0f));
	ofVec3f d = ofVec3f(0, wiggle, (wingwidth/2.0f));
	
	a.rotate(flappingAngleRad, ofVec3f(0, 0, -1));
	b.rotate(flappingAngleRad, ofVec3f(0, 0, -1));
	
	ofPushMatrix();

	ofEnableAlphaBlending();
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); 

	ofDrawAxis(1.0);
	ofTranslate(pos.x, pos.y, pos.z);
	
	//if(bIsTextured) imgLeft.draw(0, 0);
	
	// glRotatef(heading,0,1,0);
	
	if(bIsTextured) imgLeft.getTextureReference().bind();
	//ofSetColor(255, 0, 0);
	glBegin(GL_QUADS);
//		glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0);
//		glTexCoord2f(1.0,0.0); glVertex2f(1.0,0.0);
//		glTexCoord2f(1.0,1.0); glVertex2f(1.0,1.0);
//		glTexCoord2f(0.0,1.0); glVertex2f(0.0,1.0);
	glColor3fv(col); glTexCoord2f( .0,  .0);	glVertex3f(	b.x, b.y, b.z );
	glColor3fv(col); glTexCoord2f(128.0,  .0);	glVertex3f( c.x, c.y, c.z	);
	glColor3fv(col); glTexCoord2f(128.0, 128.0);	glVertex3f(	d.x, d.y, d.z );
	glColor3fv(col); glTexCoord2f( .0, 128.0);	glVertex3f(	a.x, a.y, a.z );
	glEnd();
	if(bIsTextured) imgLeft.getTextureReference().unbind();

	if(bIsTextured) imgRight.getTextureReference().bind();
	//ofSetColor(0, 255, 0);
	glBegin(GL_QUADS);
	glColor3fv(col); glTexCoord2f( .0,  .0);	glVertex3f(	-b.x, b.y, b.z );
	glColor3fv(col); glTexCoord2f(128.0,  .0);	glVertex3f(	c.x, c.y, c.z );
	glColor3fv(col); glTexCoord2f(128.0, 128.0);	glVertex3f(	d.x, d.y, d.z );
	glColor3fv(col); glTexCoord2f( .0, 128.0);	glVertex3f(	-a.x, a.y, a.z );
////	glColor3f(1.00,.451,.094); glTexCoord2f( .0,  .0);	glVertex3f(	0, 0, -wingwidth/2.0f);
////	glColor3f(1.00,.451,.094); glTexCoord2f(1.0,  .0);	glVertex3f(	-sinf(flappingAngleRad)*(-wingspan/2.0f), cosf(flappingAngleRad)*(-wingspan/2.0f), -wingwidth/2.0f);
////	glColor3f(1.00,.451,.094); glTexCoord2f(1.0, 1.0);	glVertex3f(	-sinf(flappingAngleRad)*(-wingspan/2.0f), cosf(flappingAngleRad)*(-wingspan/2.0f), wingwidth/2.0f);
////	glColor3f(1.00,.451,.094); glTexCoord2f( .0, 1.0);	glVertex3f(	0, 0, wingwidth/2.0f);
	glEnd();
	if(bIsTextured) imgRight.getTextureReference().unbind();

	ofPopMatrix();
	
/*
	glEnable(GL_TEXTURE_2D);
	butterflyTextures[type].Use();

	glPushMatrix();

	glTranslatef(position.x,position.y,position.z);

	glRotatef(heading,0,1,0);

	glScalef(5,5,5);

	glBegin(GL_QUADS);
	glColor3fv((const float*)&color1);	glTexCoord2f(1,0);	glVertex3f(	0, 0, -wingwidth/2.0f);
	glColor3fv((const float*)&color2);	glTexCoord2f(0,0);	glVertex3f(	sinf(wingAngleRad)*(-wingspan/2.0f), cosf(wingAngleRad)*(-wingspan/2.0f), -wingwidth/2.0f);
	glColor3fv((const float*)&color3);	glTexCoord2f(0,1);	glVertex3f(	sinf(wingAngleRad)*(-wingspan/2.0f), cosf(wingAngleRad)*(-wingspan/2.0f), wingwidth/2.0f);
	glColor3fv((const float*)&color4);	glTexCoord2f(1,1);	glVertex3f(	0, 0, wingwidth/2.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3fv((const float*)&color1);	glTexCoord2f(1,0);	glVertex3f(	0, 0, -wingwidth/2.0f);
	glColor3fv((const float*)&color2);	glTexCoord2f(0,0);	glVertex3f(	-sinf(wingAngleRad)*(-wingspan/2.0f), cosf(wingAngleRad)*(-wingspan/2.0f), -wingwidth/2.0f);
	glColor3fv((const float*)&color3);	glTexCoord2f(0,1);	glVertex3f(	-sinf(wingAngleRad)*(-wingspan/2.0f), cosf(wingAngleRad)*(-wingspan/2.0f), wingwidth/2.0f);
	glColor3fv((const float*)&color4);	glTexCoord2f(1,1);	glVertex3f(	0, 0, wingwidth/2.0f);
	glEnd();

	glPopMatrix();
*/
}

