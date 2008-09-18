/*
 *  gPhotoCam.cpp
 *  StopMotion2
 *
 *  Created by ole kristensen on 18/09/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 */

#include "testApp.h"

int gPhotoCam::initCamera(){
	
}

int gPhotoCam::takePicture(string picturePath){
	
}

void gPhotoCam::start(){
	startThread(true, false);   // blocking, verbose
}

void gPhotoCam::stop(){
	stopThread(); 
}

void gPhotoCam::threadedFunction(){
	
	while( isThreadRunning() != 0 ){
		if( lock() ){
						
			unlock();
			stop();
		}
	}
}
	
