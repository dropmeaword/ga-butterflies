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

	ofAddListener(ofEvents().keyPressed, this, &GameConfig::keyPressed);
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

	ofRemoveListener(ofEvents().keyPressed, this, &GameConfig::keyPressed);
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

void GameConfig::processNameFromUI(string widgetname, string val) {
	playerNames.push_back( val );
	ofLogVerbose() << widgetname << ": " << val;
}

void GameConfig::saveGameConfig() {

	playerNames.clear();
	XML.clear();
	
	vector<ofxUIWidget*> tboxes = gui2->getWidgetsOfType(OFX_UI_WIDGET_TEXTINPUT);
	for(std::vector<ofxUIWidget*>::iterator it = tboxes.begin(); it != tboxes.end(); ++it) {
		ofxUIWidget *w = *it;
		//ofLogVerbose() << "TextInput.widget: " << w->getName();
		string pname = ( ((ofxUITextInput *)w)->getTextString() );
		processNameFromUI(w->getName(), pname );
	}

	saveState2XML( CONFIG_FILENAME );
}

void GameConfig::saveState2XML(string fname) {
	XML.setValue("GAME:PLAYERS", iNumPlayers);
	XML.setValue("GAME:TEAMS", iNumTeams);
	
	XML.addTag("PLAYERLIST");
	XML.pushTag("PLAYERLIST");
	int idx = 0;
	for(std::vector<string>::iterator it = playerNames.begin(); it != playerNames.end(); ++it) {
		lastTagInsert = XML.addTag("PLAYER");
		XML.pushTag("PLAYER", lastTagInsert);
		string n = *it;
		ofLogVerbose() << "saving name in xml: " << n;
		//XML.setValue("NAME", n);
		XML.addValue("NAME", n);
		XML.addValue("TEAM", ((idx++ % iNumTeams)+1) );
		XML.popTag();
	}
	XML.popTag();
	
	XML.saveFile(fname);
}

bool GameConfig::exists() {
	bool retval = false;
	
	ofFile file(CONFIG_FILENAME);
	if( file.exists() ) {
		//game.loadConfig( file );
		ofLogNotice() << "Game configuration file found, loading game..." ;
		retval = true;
	} else {
		retval = false;
		ofLogNotice() << "Game configuration file NOT found. I will take you to the configuration screen" ;
	}
	
	return retval;
}

void GameConfig::loadGameConfig() {
	XML.loadFile(CONFIG_FILENAME);

	iNumPlayers = XML.getValue("GAME:PLAYERS", 0);
	ofLogNotice() << "GameConfig.loadGameConfig (iNumPlayers) = " << iNumPlayers;
	iNumTeams   = XML.getValue("GAME:TEAMS", 0);
	ofLogNotice() << "GameConfig.loadGameConfig (iNumTeams) = " << iNumTeams;

	XML.pushTag("PLAYERLIST");
	int idx = 0;
	int i = XML.getNumTags("PLAYER");
	ofLogVerbose() << "loadGameConfig number of PLAYER tags = " << i;
	for(; i > 0; --i) {
		string pname = XML.getValue("PLAYER:NAME", "not set", idx);
		int pteam = XML.getValue("PLAYER:TEAM", -1, idx);
		ofLogVerbose() << "player name:  " << pname << ", team: " << pteam;
		playerNames.push_back(pname);
		idx++;
	}
	XML.popTag();
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
			/*
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
			*/       
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
	else if (name == "SAVE") 
	{
			ofxUILabelButton *button = (ofxUILabelButton *) e.widget; 
			bool val = button->getValue();
			if( (val == true) && ( (iNumTeams > 0) && (iNumPlayers > 0) ) ) {
				saveGameConfig();
			}
	}
} // guiEvent( ofUIEventArgs & )

void GameConfig::keyPressed(ofKeyEventArgs& eventArgs) {
	int key = eventArgs.key;

	switch(key) {
		case 's':
		case 'S':
			//saveGameConfig();
			break;
	} // switch
}

