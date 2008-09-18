/*
 *  gPhotoCam.h
 *  StopMotion2
 *
 *  Created by ole kristensen on 18/09/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 */

#ifndef _G_PHOTO_CAM_H
#define _G_PHOTO_CAM_H
#include "gphoto2.h"

class gPhotoCam : public ofxThread {
public:
	
	int initCamera();
	int takePicture(string picturePath);
	
	gPhotoCam();
	
	void start();
	void stop();
	
	void threadedFunction();
	
private:
	
	Camera gpCam;
	GPContext gpContext;
};
#endif

