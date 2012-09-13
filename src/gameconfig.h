#ifndef __GAMECONFIG_H__
#define __GAMECONFIG_H__

#include "ofMain.h"
#include "ofxUI.h"
#include "activity.h"

#define CONFIG_FILENAME ofToDataPath("game.xml")

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
	
	bool exists();
	
	void saveGameConfig();
	void loadGameConfig();

	void guiEvent(ofxUIEventArgs &e);
	void keyPressed(ofKeyEventArgs& eventArgs);
	void processNameFromUI(string widgetname, string val);
	void saveState2XML(string fname);

	ofxUICanvas *gui;
	ofxUICanvas *gui2;
	
	ofxXmlSettings XML; 
	ePanel currentPanel;
	
	int lastTagInsert;
	
	int iNumPlayers;
	int iNumTeams;
	
	vector<string> playerNames;
}; // class 

#endif // __GAMECONFIG_H__