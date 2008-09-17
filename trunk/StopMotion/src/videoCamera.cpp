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
	cornerNE.set(-1.0f,-1.0f);
	cornerSE.set(-1.0f,-1.0f);
	cornerSW.set(-1.0f,-1.0f);
	cornerNW.set(-1.0f,-1.0f);
}

void ofxVideoCamera::update(ofxPoint2f _currentLocation){
	if(getTransformedPoint(_currentLocation)==ofxPoint2f(-1.0f,-1.0f)){
		// der er ikke kalibreret endnu
	} else {
		//lav alt Matriceopdateringen...
	}
}

void ofxVideoCamera::draw(){
	if(getTransformedPoint(ofxPoint2f(0.0f,0.0f))==ofxPoint2f(-1.0f,-1.0f)){
		// der er ikke kalibreret endnu
	}
	// tegn en grafik under kalibreringen ---
}

ofxPoint2f ofxVideoCamera::getTransformedPoint(ofxPoint2f _coordinate){
	
	//check if all corners have been initialised
	if(cornerNE==ofxPoint2f(-1.0f,-1.0f) || cornerSE==ofxPoint2f(-1.0f,-1.0f) || cornerSW==ofxPoint2f(-1.0f,-1.0f) || cornerNW==ofxPoint2f(-1.0f,-1.0f) ){
		return ofxPoint2f(-1.0f,-1.0f);
	} else {
		//lav alt Matriceopslag...
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
	
	if(_coordinate.x > 0.6f && _coordinate.y > 0.6f*ASPECTRATIO){
		cornerNE.set(_coordinate);
		return 1;
	} else if(_coordinate.x > 0.6f && _coordinate.y < 0.4f*ASPECTRATIO){
		cornerSE.set(_coordinate);
		return 2;
	} else if(_coordinate.x < 0.4f && _coordinate.y < 0.4f*ASPECTRATIO){
		cornerSW.set(_coordinate);
		return 3;
	} else if(_coordinate.x < 0.4f && _coordinate.y > 0.6f*ASPECTRATIO){
		cornerNW.set(_coordinate);
		return 4;
	} else {
		return -1;
	}
}