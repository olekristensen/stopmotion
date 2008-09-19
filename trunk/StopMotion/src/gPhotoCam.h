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
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

class gPhotoCam : public ofxThread {
public:
	
	int initCamera();
	int takePicture(string picturePath);
	
	gPhotoCam();
	
	void start();
	void stop();
	
	void threadedFunction();
	
private:
	int infp, outfp;
	char buf[1024];
};
#endif

