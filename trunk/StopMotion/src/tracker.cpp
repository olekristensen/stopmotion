/*
 *  tracker.cpp
 *  StopMotion2
 *
 *  Created by Jonas Jongejan on 16/09/08.
 *  Modified by Ole Kristensen today
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
	loc.x = 0;
	loc.y = 0;
	pointMoved = false;
	
	font.loadFont("Candara Bold.ttf", 12); 
	
}
void ofxTracker::update(){
	bool bNewFrame = false;
	
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
	
	if (bNewFrame){
		
		colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
        grayImage = colorImg;
		
		
		// take the abs value of the difference between background and incoming and then threshold:
		//grayDiff.absDiff(grayBg, grayImage);
		grayImage.threshold(threshold);
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage, 20, (340*240)/3, 10, false);	// find holes
		if(contourFinder.nBlobs > 0){
			ofPoint location = contourFinder.blobs[0].centroid;
			if(loc.x != location.x || loc.y != location.y){
				loc.y = 240-location.y;
				loc.x = 320-location.x;
				pointMoved = true;
			} else {
				pointMoved = false;
			}
		}
	}
	
}

void ofxTracker::draw(){
	ofDisableAlphaBlending();

	//glTranslated(-10, -10, 0);
	colorImg.draw(0,0, ofGetWidth(), ofGetHeight());
	//contourFinder.draw(0,0);
	//glRotated(-90, 0, 0, 1);
	//glTranslated(-ofGetWidth(), 0, 0);
	//ofEllipse(loc.x, loc.y, 10, 10);
	ofSetColor(255, 255, 255, 255);
	font.drawString(ofToString((float)loc.x/320, 1)+ " - "+ofToString(loc.y)+" - "+ofToString((1.0-(float)loc.y/240),1) ,ofGetWidth()-200, ofGetHeight()-15);
	ofEnableAlphaBlending();

}

ofxPoint2f ofxTracker::getCurrentLocation(ofxVideoCamera cam){
	return ofxPoint2f(cam.getTransformedPoint(ofxPoint2f(1.0-((float)loc.x/320.0), ASPECTRATIO*(1.0-((float)loc.y/(240.0))))));
}