/*
 *  gameconfig.cpp
 *  ga-butterflies
 *
 *  Created by zilog on 9/1/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "gameconfig.h"

GameConfig::GameConfig() : Activity("GameConfig") {
	// intialize UI elements
	int panelWidth = 500;
	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
	float length = 255-xInit; 
		
	gui = new ofxUICanvas(ofGetScreenWidth() - panelWidth, 0, panelWidth, ofGetScreenHeight());
	gui->addWidgetDown(new ofxUILabel("PARTICIPATION", OFX_UI_FONT_MEDIUM)); 
	gui->addLabel("LABEL_PLAYERS", "How many players?", OFX_UI_FONT_MEDIUM);
	gui->addTextInput("INPUT_PLAYERS", "", length-xInit);
	gui->addLabel("LABEL_TEAMS", "How many teams?", OFX_UI_FONT_MEDIUM);
	gui->addTextInput("INPUT_TEAMS", "", length-xInit);
	
	gui->addWidgetDown(new ofxUILabelButton(false, "NEXT >>", OFX_UI_FONT_SMALL));
	
//	gui->addWidgetDown(new ofxUIToggle(16, 16, bInvert, "INVERT BINARY IMG"));
	
	ofAddListener(gui->newGUIEvent, this, &GameConfig::guiEvent);
	gui->setVisible( true );
//	ofLogVerbose() << "Trying to load GUI settings for " + this->name;
//	// load last settings
//	gui->loadSettings("GUI/param.photobooth.xml");
}

GameConfig::~GameConfig() {
}

void GameConfig::show() {
	ofAddListener(gui->newGUIEvent, this, &GameConfig::guiEvent);
	gui->setVisible( true );
	Activity::show();
}

void GameConfig::hide() {
	ofRemoveListener(gui->newGUIEvent, this, &GameConfig::guiEvent);
	gui->setVisible( false );
	Activity::hide();
}

void GameConfig::setup() {
}

void GameConfig::update() {
}

void GameConfig::draw() {
}

void GameConfig::guiEvent(ofxUIEventArgs &e) {
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl;

	if(name == "INPUT_TEAMS") {
			ofxUITextInput *textinput = (ofxUITextInput *) e.widget; 
			string output = textinput->getTextString(); 
			cout << "TEAMS " << output << endl; 
	}
	else if(name == "INPUT_PLAYERS")
	{
			ofxUITextInput *textinput = (ofxUITextInput *) e.widget; 
			if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
			{
					cout << "ON ENTER: "; 
//            ofUnregisterKeyEvents((testApp*)this); 
			}
			else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
			{
					cout << "ON FOCUS: "; 
			}
			else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
			{
					cout << "ON BLUR: "; 
//            ofRegisterKeyEvents(this);             
			}        
			string output = textinput->getTextString(); 
			cout << "PLAYERS " << output << endl; 
	}
}
