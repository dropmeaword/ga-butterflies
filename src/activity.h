#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__

#include "ofMain.h"

class Activity {
		static int iid;  // instance id
	public:
		string name;
		
		Activity();
		Activity(string str);
		virtual ~Activity();
		
		void setup();
		void update();
		void draw();
};

#endif // __ACTIVITY_H__
