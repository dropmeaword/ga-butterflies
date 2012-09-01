#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__

#include "ofMain.h"

class Activity {
	protected:
		static int iid;  // instance id
		bool bHidden;
		
	public:
		string name;
		
		Activity();
		Activity(string str);
		virtual ~Activity();
		
		void show();
		void hide();
		
		void setup();
		void update();
		void draw();
};

#endif // __ACTIVITY_H__
