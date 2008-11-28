/*
 *  canonRemoteCam.h
 *  StopMotion2
 *
 *  Created by ole kristensen on 27/11/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 */


#ifndef _CANON_REMOTE_CAM_H
#define _CANON_REMOTE_CAM_H
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

class canonRemoteCam : public ofxThread {
public:
	
	int initCamera();
	int takePicture();
	string filenameOfLastPicture();
	canonRemoteCam();
		
	int getState();
	void setState(int _state);
	
	void start(imageStore* _imgStore);
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
	int imageCount;
	imageStore* imgStore;
	int scaleCount;
	imageStore scaleStore;
	ofImage scaleImg;
	
};
#endif

