/*
 *  imageStore.cpp
 *  StopMotion2
 *
 *  Created by ole kristensen on 18/09/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 */

#include "imageStore.h"

imageStore::imageStore(string _folderPath, string _ext){
	folderPath = _folderPath;
	ext = _ext;
}

int imageStore::getImageCount(){
	ofxDirList.reset();
	ofxDirList.setVerbose(true);
	ofxDirList.allowExt(ext);
	return ofxDirList.listDir(folderPath);
}

string imageStore::addImage(*ofImage){
	// not implemented
}

string imageStore::replaceImage(*ofImage, int _pos){
	// not implemented
}

string imageStore::replaceImage(*ofImage, string _filename){
	// not implemented
}

string imageStore::getFilenameFromPos(int _pos){
	int fileCount;
	ofxDirList.reset();
	ofxDirList.setVerbose(true);
	ofxDirList.allowExt(ext);
	fileCount = ofxDirList.listDir(folderPath);
	if(pos <= fileCount){
		return ofxDirList.getName(_pos);
	} else {
		return NULL;
	}
}

bool imageStore::imageExits(string _filename){
	ofxDirList.reset();
	ofxDirList.setVerbose(true);
	ofxDirList.allowExt(ext);
	fileCount = ofxDirList.listDir(folderPath);
	for(int i = 0; i < fileCount; i++){
		if(ofxDirList.getName(i).compare(_filename)){
		   return true;
		}
	}
	return false;
}

bool imageStore::imageExists(int _pos){
	return (pos <= getImageCount());
}

ofImage imageStore::getImage(int _pos){
	return getImage(getFilenameFromPos(_pos));
}

ofImage imageStore::getImage(string _filename){
	ofImage retImage;
	if (filename != NULL){
		ofImage.loadImage(folderPath + "/" + _filename);
	}
	return retImage;
}

ofImage imageStore::getLastImage(){
	return getImage(getImageCount());
}

bool imageStore::deleteImage(int _pos){
	return deleteImage(getFilenameFromPos(_pos));
}

bool imageStore::deleteImage(int _filename){
	if(imageExits(filename)){
		string cmd = "rm "+ folderPath + "/" + filename;
		int ret = system((const char *)cmd.c_str());
		cout<<"remove newest image "<<cmd<<endl;
		cout<<"return "<<ret<<endl;
		if (ret == 0){
			return true;
		}
	}
	return false;
}
