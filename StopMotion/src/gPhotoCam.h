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
	int takePicture();
	string filenameOfLastPicture();
	gPhotoCam();
	
	int getState();
	void setState(int _state);
	
	void start();
	void stop();

	int state;

	void threadedFunction();
		
	enum{START_CAPTURE, CAPTURING, INTERRUPTED, CAPTURE_COMPLETE, START_DOWNLOAD, DOWNLOAD_COMPLETE, READY, BUSY};
	
private:
	int infp, outfp;
	char buf[10000];
	string filename;
	string pathname;
	string commandline;
	string bufStr;
	int milliseconds;
	bool hasCded;
};
#endif

