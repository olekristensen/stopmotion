/*
 *  tracker.cpp
 *  StopMotion2
 *
 *  Created by Jonas Jongejan on 16/09/08.
 * 
 *
 */
#include "testApp.h"

void ofxTracker::setup(){
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(320,240);
	
	colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	
	threshold = 100;
}

void ofxTracker::update(){
	bool bNewFrame = false;

	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
	
	if (bNewFrame){
		
		colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
		
        grayImage = colorImg;

		
		// take the abs value of the difference between background and incoming and then threshold:
//		grayDiff.absDiff(grayBg, grayImage);
		grayImage.threshold(threshold);
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, false);	// find holes
	}
}