/*
 *  fileSystem.h
 *  StopMotion2
 *
 *  Created by ole kristensen on 18/09/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 */


#ifndef _SM_FILESYSTEM_H
#define _SM_FILESYSTEM_H
#include "ofxDirList.h"


class imageStore : public ofSimpleApp {
public:
	
	imageStore(string _folderPath, string _ext ="jpg");
	
	string folderPath;
	string ext;
	
	int getImageCount();
	
	string addImage(*ofImage);
	string replaceImage(*ofImage, int _pos);
	string replaceImage(*ofImage, string _filename);
	
	string getFilenameFromPos(int _pos);
	ofImage getImage(int _pos);
	ofImage getImage(string _filename);
	ofImage getLastImage();
	
	bool imageExists(int _pos);
	bool imageExists(string _filename);
	bool deleteImage(int _pos);
	bool deleteImage(string _filename);
	
private:
	
	ofxDirList ofxDirList;
	
};
#endif