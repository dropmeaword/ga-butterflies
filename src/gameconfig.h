#ifndef __GAMECONFIG_H__
#define __GAMECONFIG_H__

#include "ofMain.h"
#include "ofxUI.h"
#include "activity.h"

class GameConfig : public Activity {
public:

	enum ePanel { PANEL_GAME, PANEL_PLAYERS };
	
	GameConfig();
	virtual ~GameConfig();
	
	void setup();
	void update();
	void draw();
	void show();
	void hide();
	
	void clear();
	
	void setGUI1();
	void setGUI2();

	void setCurrentPanel(ePanel panel);
	
	void guiEvent(ofxUIEventArgs &e);

	ofxUICanvas *gui;
	ofxUICanvas *gui2;
	
	ePanel currentPanel;
	
	int iNumPlayers;
	int iNumTeams;
	
	vector<string> playerNames;
}; // class 

#endif // __GAMECONFIG_H__