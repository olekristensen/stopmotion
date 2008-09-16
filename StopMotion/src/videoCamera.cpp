/*
 *  videoCamera.cpp
 *  StopMotion2
 *
 *  Created by ole kristensen on 16/09/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 */

#include "testApp.h"

void ofxVideoCamera::setup(){
	cornerNE.set(-1.0f,-1.0f);
	cornerSE.set(-1.0f,-1.0f);
	cornerSW.set(-1.0f,-1.0f);
	cornerNW.set(-1.0f,-1.0f);
}

void ofxVideoCamera::update(){
	
}

void ofxVideoCamera::draw(){
	
}

ofxPoint2f ofxVideoCamera::getTransformedPoint(ofxPoint2f _coordinate){
	
	//check if all corners have been initialised
	if(cornerNE==ofxPoint2f(-1.0f,-1.0f) || cornerSE==ofxPoint2f(-1.0f,-1.0f) || cornerSW==ofxPoint2f(-1.0f,-1.0f) || cornerNW==ofxPoint2f(-1.0f,-1.0f) ){
		return ofxPoint2f(-1.0f,-1.0f);
	} else {
		
	}
}

ofxPoint2f ofxVideoCamera::getUnTransformedPoint(ofxPoint2f _coordinate){
	
	//check if all corners have been initialised
	if(cornerNE==ofxPoint2f(-1.0f,-1.0f) || cornerSE==ofxPoint2f(-1.0f,-1.0f)  || cornerSW==ofxPoint2f(-1.0f,-1.0f) || cornerNW==ofxPoint2f(-1.0f,-1.0f) ){
		return ofxPoint2f(-1.0f,-1.0f);
	} else {
		
	}
}

int ofxVideoCamera::setCornerPoint(ofxPoint2f _coordinate){
	
	//find which corner we're in and return an int representing the corner, if it was within the limits,
	//else return a -1;
	
	if(_coordinate.x > 0.6f && _coordinate.y > 0.6f){
		cornerNE.set(_coordinate);
		return 1;
	} else if(_coordinate.x > 0.6f && _coordinate.y < 0.4f){
		cornerSE.set(_coordinate);
		return 2;
	} else if(_coordinate.x < 0.4f && _coordinate.y < 0.4f){
		cornerSW.set(_coordinate);
		return 3;
	} else if(_coordinate.x < 0.4f && _coordinate.y > 0.6f){
		cornerNW.set(_coordinate);
		return 4;
	} else {
		return -1;
	}
}