#include "activity.h"


int Activity::iid = 1;

Activity::Activity() {
	name = "default" + (iid++);
}

Activity::Activity(string str) {
	iid++;
	name = str;
}

Activity::~Activity() {
}
		
void Activity::setup() {
}

void Activity::update() {
}

void Activity::draw() {
}

void Activity::show() {
	bHidden = false;
}

void Activity::hide() {
	bHidden = true;
}
