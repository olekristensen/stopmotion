#include "testApp.h"

ofxStopmotionMarker::ofxStopmotionMarker(){
	loc.x = 0;
	loc.y = 0;
	markerImg.loadImage("Marker.png");

}

void ofxStopmotionMarker::draw(){
	//Draw marker image
	ofFill();
	ofSetColor(255, 255, 255,60);
	float markerSize = CAPTURERADIUS*4*ofGetWidth();
	markerImg.draw(loc.x*ofGetWidth()-markerSize*0.5, loc.y*ofGetWidth()-markerSize*0.5,markerSize,markerSize);	
	
	ofDisableAlphaBlending();
	if (percent > 0){

		int resolution = 200;
		ofSetCircleResolution(resolution);
		
		int numBricks = round(resolution*percent);
		double radianStep = (double)TWO_PI/resolution;
		
		if(captureState == CAPTURE_CAPTURING)
			glColor3f(0, 0, 0);
		else if(captureState == CAPTURE_IDLE)
			glColor3f(255,255,255);
		else if(captureState == CAPTURE_INTERRUPTED)
			glColor3f(255,60,60);
		else if(captureState == CAPTURE_LOADING){
			ofFill();
			ofSetColor(0, 0, 0);
			ofCircle(loc.x*ofGetWidth(), loc.y*ofGetWidth(), CAPTURERADIUS*ofGetWidth());
			glColor3f(255,255,255);
		}
		
		glBegin(GL_TRIANGLES);
		for(int i=-resolution/4;i<numBricks-resolution/4; i++){
			glVertex2f(loc.x*ofGetWidth(), loc.y*ofGetWidth());
			glVertex2f(loc.x*ofGetWidth()+(CAPTURERADIUS)*cos(i*radianStep)*ofGetWidth(), loc.y*ofGetWidth()+(CAPTURERADIUS)*sin(i*radianStep)*ofGetWidth());
			glVertex2f(loc.x*ofGetWidth()+(CAPTURERADIUS)*cos((i-1)*radianStep)*ofGetWidth(), loc.y*ofGetWidth()+(CAPTURERADIUS)*sin((i-1)*radianStep)*ofGetWidth());
		}
		glEnd();
	}
}
