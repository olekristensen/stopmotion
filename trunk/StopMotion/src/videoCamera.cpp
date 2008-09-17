/*
 *  videoCamera.cpp
 *  StopMotion2
 *
 *  Created by ole kristensen on 16/09/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 *  HUSKAT gange med aspect i Y
 *
 */

#include "testApp.h"

void ofxVideoCamera::setup(){
	
	// set dstPoints unInitialised (-1.0)
	for (int i = 0; i < 4; i++){
		dstPoints[i].x = -1.0f;
		dstPoints[i].y = -1.0f;
	}
	
	// set srcPoints
	
	srcPoints[0].y = ASPECTRATIO;	//North
	srcPoints[0].x = 1.0f;			//East
	
	srcPoints[1].y = 0.0f;			//South
	srcPoints[1].x = 1.0f;			//East
	
	srcPoints[2].y = 0.0f;			//South
	srcPoints[2].x = 0.0f;			//West
	
	srcPoints[3].y = ASPECTRATIO;	//North
	srcPoints[3].x = 0.0f;			//West
	
}

void ofxVideoCamera::loadDstPoints(ofxPoint2f loadPoints[4]){
	
	// set dstPoints
	for (int i = 0; i < 4; i++){
		dstPoints[i].x = loadPoints[i].x;
		dstPoints[i].y = loadPoints[i].y;
	}
	
	state = STATE_CALIBRATED_ALL;
	
}

void ofxVideoCamera::update(ofxPoint2f _currentLocation, bool captureCornerPoint){
	
	// if a corner should be set...
	if(captureCornerPoint){
		state=STATE_CALIBRATING;
		int calibratedCorner = 0;
		calibratedCorner = setCornerPoint(_currentLocation);
		if(calibratedCorner > 0){
			state = calibratedCorner;
		}
	}
	
	if(state != STATE_MATRIX_DONE){
		// see if all four corners are set yet...
		if(state != STATE_CALIBRATED_ALL){
			bool allCornersSet = true;
			for (int i = 0; i < 4; i++){
				if(dstPoints[i].x == -1.0f && dstPoints[i].y == -1.0f){
					allCornersSet = false;
				}
			}
			if (allCornersSet){
				state = STATE_CALIBRATED_ALL;
			}
		}
		if(state == STATE_CALIBRATED_ALL){
			coordWarper.calculateMatrix(dstPoints,srcPoints);
			state = STATE_MATRIX_DONE;
		}
	}
}

void ofxVideoCamera::draw(ofxPoint2f _currentLocation){
	if(state != STATE_MATRIX_DONE){
		if(_currentLocation.x > 0.6f && _currentLocation.y > 0.6f*ASPECTRATIO){
			if(state == STATE_CALIBRATED_NE){
				ofSetColor(255,255,255);
			} else {
				ofSetColor(255,255,0);
			}
			ofLine(_currentLocation.x * (float)ofGetWidth(),_currentLocation.y * (float) ofGetWidth(),1.0f * (float)ofGetWidth(),ASPECTRATIO * (float) ofGetWidth());
		} else if(_currentLocation.x > 0.6f && _currentLocation.y < 0.4f*ASPECTRATIO){
			if(state == STATE_CALIBRATED_SE){
				ofSetColor(255,255,255);
			} else {
				ofSetColor(255,255,0);
			}
			ofLine(_currentLocation.x * (float)ofGetWidth(),_currentLocation.y * (float) ofGetWidth(),1.0f * (float)ofGetWidth(),0.0f * (float) ofGetWidth());
		} else if(_currentLocation.x < 0.4f && _currentLocation.y < 0.4f*ASPECTRATIO){
			if(state == STATE_CALIBRATED_SW){
				ofSetColor(255,255,255);
			} else {
				ofSetColor(255,255,0);
			}
			ofLine(_currentLocation.x * (float)ofGetWidth(),_currentLocation.y * (float) ofGetWidth(),0.0f * (float)ofGetWidth(),0.0f * (float) ofGetWidth());
		} else if(_currentLocation.x < 0.4f && _currentLocation.y > 0.6f*ASPECTRATIO){
			if(state == STATE_CALIBRATED_NW){
				ofSetColor(255,255,255);
			} else {
				ofSetColor(255,255,0);
			}
			ofLine(_currentLocation.x * (float)ofGetWidth(),_currentLocation.y * (float) ofGetWidth(),0.0f * (float)ofGetWidth(),ASPECTRATIO * (float) ofGetWidth());
		} else {
			ofSetColor(128,128,0);
			ofLine(_currentLocation.x * (float)ofGetWidth(),_currentLocation.y * (float) ofGetWidth(),1.0f * (float)ofGetWidth(),ASPECTRATIO * (float) ofGetWidth());
			ofLine(_currentLocation.x * (float)ofGetWidth(),_currentLocation.y * (float) ofGetWidth(),1.0f * (float)ofGetWidth(),0.0f * (float) ofGetWidth());
			ofLine(_currentLocation.x * (float)ofGetWidth(),_currentLocation.y * (float) ofGetWidth(),0.0f * (float)ofGetWidth(),0.0f * (float) ofGetWidth());
			ofLine(_currentLocation.x * (float)ofGetWidth(),_currentLocation.y * (float) ofGetWidth(),0.0f * (float)ofGetWidth(),ASPECTRATIO * (float) ofGetWidth());
		}
	}
}

ofxPoint2f ofxVideoCamera::getTransformedPoint(ofxPoint2f _coordinate){
	if(state == STATE_MATRIX_DONE){
		return coordWarper.transform(_coordinate.x,_coordinate.y);
	} else {
		return _coordinate;
	}
}

int ofxVideoCamera::setCornerPoint(ofxPoint2f _coordinate){
	
	//find which corner we're in and return an int representing the corner, if it was within the limits,
	//else return a -1;
	
	if(_coordinate.x > 0.6f && _coordinate.y > 0.6f*ASPECTRATIO){
		dstPoints[0].set(_coordinate);
		return 1;
	} else if(_coordinate.x > 0.6f && _coordinate.y < 0.4f*ASPECTRATIO){
		dstPoints[1].set(_coordinate);
		return 2;
	} else if(_coordinate.x < 0.4f && _coordinate.y < 0.4f*ASPECTRATIO){
		dstPoints[2].set(_coordinate);
		return 3;
	} else if(_coordinate.x < 0.4f && _coordinate.y > 0.6f*ASPECTRATIO){
		dstPoints[3].set(_coordinate);
		return 4;
	} else {
		return -1;
	}
}