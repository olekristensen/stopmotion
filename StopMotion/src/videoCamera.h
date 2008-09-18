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
	
	ofxPoint2f srcPoints[4];
	ofxPoint2f dstPoints[4];
	
	void setup(ofxXmlSettings XML);
	void update(ofxPoint2f _currentLocation, bool captureCornerPoint);
	void draw(ofxPoint2f _currentLocation);
	
	void loadDstPoints(ofxPoint2f loadPoints[4]);
	void loadXml(ofxXmlSettings XML);
	
	ofxPoint2f getTransformedPoint(ofxPoint2f coordinate);
	int setCornerPoint(ofxPoint2f coordinate);
		
	enum { STATE_CALIBRATING, STATE_CALIBRATED_NE, STATE_CALIBRATED_SE, STATE_CALIBRATED_SW, STATE_CALIBRATED_NW, STATE_CALIBRATED_ALL, STATE_MATRIX_DONE };
	
	int state;
	
	coordWarping coordWarper;
	
	ofImage[4] calibrationImages;
};

#endif