#ifndef __GAMECONFIG_H__
#define __GAMECONFIG_H__

#include "ofMain.h"
#include "ofxUI.h"
#include "activity.h"

class GameConfig : public Activity {
public:

	GameConfig();
	virtual ~GameConfig();
	
	void setup();
	void update();
	void draw();
	void show();
	void hide();
	
	void guiEvent(ofxUIEventArgs &e);

	ofxUICanvas *gui;
	
	int iNumPlayers;
	int iNumTeams;
	
	vector<string> playerNames;
}; // class 

#endif // __GAMECONFIG_H__