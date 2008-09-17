/* 
 *  videoCamera.h
 *  StopMotion2
 * 
 *  Created by ole kristensen on 16/09/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 * 
 */

#ifndef _VIDEO_CAMERA_H
#define _VIDEO_CAMERA_H
#include "coordWarping.h"

class ofxVideoCamera : public ofSimpleApp {
public:
	
	ofxPoint2f cornerNE, cornerSE, cornerSW, cornerNW;
		
	void setup();
	void update(ofxPoint2f _currentLocation);
	void draw();
	
	ofxPoint2f getTransformedPoint(ofxPoint2f coordinate);
	ofxPoint2f getUnTransformedPoint(ofxPoint2f coordinate);
	int setCornerPoint(ofxPoint2f coordinate);
	
};

#endif