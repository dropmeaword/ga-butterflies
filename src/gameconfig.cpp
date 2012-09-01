#include "gameconfig.h"

GameConfig::GameConfig() : Activity("GameConfig") {
	iNumPlayers = 0;
	iNumTeams = 0;
	setCurrentPanel( PANEL_GAME );
}

GameConfig::~GameConfig() {
}

void GameConfig::setCurrentPanel(ePanel panel) {
	switch(panel) {
		case PANEL_GAME:
			setGUI1();
			break;
		case PANEL_PLAYERS:
			setGUI2();
			break;
	}
	
	currentPanel = panel;
}

void GameConfig::setGUI1() {
	// intialize UI elements
	int panelWidth = 350;
	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
	float length = 255-xInit; 
		
	gui = new ofxUICanvas(0, 0, panelWidth, ofGetScreenHeight());
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

void GameConfig::setGUI2() {
	int xx, yy;
	stringstream ss1, ss2, ss3;
	
	// intialize UI elements
	int panelWidth = ofGetScreenWidth() - 350;
	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
	float length = 255-xInit; 

	xx = xInit;
	yy = xInit;
			
	gui2 = new ofxUICanvas(350, 0, panelWidth, ofGetScreenHeight());

	// set-up all the edit boxes
	for(int i = 1; i <= iNumPlayers; i++) {
		// clear
		ss1.str(std::string());
		ss2.str(std::string());
		ss3.str(std::string());
		
		// build strings
		ss1 << "LABEL_PLAYER_" <<  i;
		string lblname = ss1.str();
		ss2 << "PLAYER_" <<  i;
		string plyname = ss2.str();
		ss3 << "Name of player " << i;
		string lbl = ss3.str();
		
		//gui2->addLabel(lblname, lbl, OFX_UI_FONT_MEDIUM);
		gui2->addWidget( new ofxUILabel(xx, yy, lblname, lbl, OFX_UI_FONT_MEDIUM) );
		yy += 24;
		gui2->addWidget( new ofxUITextInput(plyname, "", length-xInit, 24, xx, yy) );
		yy += 30;
		//gui2->addTextInput(plyname, "", length-xInit);
		
		// every ten boxes shift one column to the right
		if( (i % 15) == 0) {
			xx += 250;
			yy = xInit;
			//gui2->resetPlacer();
		}
	}
	
	// add button to save game configuration
	gui->addWidget(new ofxUILabelButton(xInit, (ofGetScreenHeight() - 200), 150, false, "SAVE", OFX_UI_FONT_SMALL));
	gui->addWidget(new ofxUILabelButton(xInit, (ofGetScreenHeight() - 160), 150, false, "EXIT CONFIG", OFX_UI_FONT_SMALL));
	
	ofAddListener(gui2->newGUIEvent, this, &GameConfig::guiEvent);
	gui2->setVisible( true );
}

void GameConfig::show() {
	switch(currentPanel) {
		case PANEL_GAME:
			ofAddListener(gui->newGUIEvent, this, &GameConfig::guiEvent);
			gui->setVisible( true );
			break;
			
		case PANEL_PLAYERS:
			ofAddListener(gui2->newGUIEvent, this, &GameConfig::guiEvent);
			gui2->setVisible( true );
			break;
	}
	Activity::show();
}

void GameConfig::hide() {
	switch(currentPanel) {
		case PANEL_GAME:
			ofRemoveListener(gui->newGUIEvent, this, &GameConfig::guiEvent);
			gui->setVisible( false );
			break;
		
		case PANEL_PLAYERS:
			ofRemoveListener(gui2->newGUIEvent, this, &GameConfig::guiEvent);
			gui2->setVisible( false );
			break;
	}
	Activity::hide();
}

void GameConfig::setup() {
}

void GameConfig::update() {
}

void GameConfig::draw() {
}

void GameConfig::clear() {
	iNumPlayers = 0;
	iNumTeams = 0;
	playerNames.clear();
}

void GameConfig::guiEvent(ofxUIEventArgs &e) {
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl;

	if(name == "INPUT_TEAMS") {
			ofxUITextInput *textinput = (ofxUITextInput *) e.widget; 
			string output = textinput->getTextString(); 
			cout << "TEAMS " << output << endl; 
			iNumTeams = atoi(output.c_str());
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
			iNumPlayers = atoi(output.c_str());
	}
	else if (name == "NEXT >>") 
	{
			ofxUILabelButton *button = (ofxUILabelButton *) e.widget; 
			bool val = button->getValue();
			if( (val == true) && ( (iNumTeams > 0) && (iNumPlayers > 0) ) ) {
				setCurrentPanel( PANEL_PLAYERS );
			}
	}
} // guiEvent( ofUIEventArgs & )
