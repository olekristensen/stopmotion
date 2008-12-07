/*
 *  canonRemoteCam.cpp
 *  StopMotion2
 *
 *  Created by ole kristensen on 27/11/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 */

#include "testApp.h"

canonRemoteCam::canonRemoteCam(){
	state = BUSY;
	hasCded = false;
}

int canonRemoteCam::initCamera(){
	
}

string canonRemoteCam::filenameOfLastPicture(){
	return filename;
}


int canonRemoteCam::getState(){
	return state;
}

void canonRemoteCam::setState(int _state){
	state = _state;
}

int canonRemoteCam::takePicture(){
	if(state == READY)
		state=START_CAPTURE;
}

void canonRemoteCam::start(imageStore* _imgStore){
	imgStore = _imgStore;
	scaleStore.folderPath = ofToDataPath("scaler");
	scaleStore.ext = "jpg";
	startThread(true, false);   // blocking, verbose
	state = READY;	
}

void canonRemoteCam::stop(){
	stopThread(); 
}

void canonRemoteCam::threadedFunction(){
	
	while( isThreadRunning() != 0 ){
		
		if( lock() ){
			switch (state) {
				case START_CAPTURE:
					state = CAPTURING;
					//tag billede
					int rv = system("/usr/bin/osascript /Volumes/Arkivet/Development/src/stopmotion/data/canonRemoteCamTrigger.scpt");
					break;
				case CAPTURING:
					state = CAPTURE_COMPLETE;
					ofSleepMillis(2000);
					break;
				case CAPTURE_COMPLETE:
					imageCount = imgStore->getImageCount();
					//vent på overførsel
					ofSleepMillis(500);
					break;
				case START_DOWNLOAD:
					//skalering
					if (scaleStore.getImageCount() > 0){
						int rv = system("/usr/bin/osascript /Volumes/Arkivet/Development/src/stopmotion/data/scaleImages.scpt");
						ofSleepMillis(2000);
					}
					if(imageCount < imgStore->getImageCount()){
						state = DOWNLOAD_COMPLETE;
						ofSleepMillis(1000);
					}
					ofSleepMillis(100);
					break;
				case INTERRUPTED:
					state = READY;
					break;
				case DOWNLOAD_COMPLETE:					
					break;
				case READY:
					ofSleepMillis(100);
					break;
				default:
					break;
			}
			unlock();
		}
		
	}
}

