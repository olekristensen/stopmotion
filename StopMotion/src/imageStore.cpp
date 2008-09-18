/*
 *  imageStore.cpp
 *  StopMotion2
 *
 *  Created by ole kristensen on 18/09/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 */

#include "testApp.h"

imageStore::imageStore(string _folderPath, string _ext ){
	folderPath = _folderPath;
	ext = _ext;
}

imageStore::imageStore(){
	folderPath = ".";
	ext = "";
}


int imageStore::getImageCount(){
	dirList.reset();
	dirList.setVerbose(true);
	dirList.allowExt(ext);
	return dirList.listDir(folderPath);
}

string imageStore::addImage(ofImage *image){
	// not implemented
	return "";
}

string imageStore::replaceImage(ofImage *image, int _pos){
	// not implemented
	return "";
}

string imageStore::replaceImage(ofImage *image, string _filename){
	// not implemented
	return "";
}

string imageStore::getFilenameFromPos(int _pos){
	int fileCount;
	dirList.reset();
	dirList.setVerbose(true);
	dirList.allowExt(ext);
	fileCount = dirList.listDir(folderPath);
	if(_pos <= fileCount){
		return dirList.getName(_pos-1);
	} else {
		return "";
	}
}

bool imageStore::imageExists(string _filename){
	dirList.reset();
	dirList.setVerbose(true);
	dirList.allowExt(ext);
	int fileCount = dirList.listDir(folderPath);
	for(int i = 0; i < fileCount; i++){
		if(dirList.getName(i).compare(_filename)){
		   return true;
		}
	}
	return false;
}

bool imageStore::imageExists(int _pos){
	return (_pos <= getImageCount());
}

ofImage imageStore::getImage(int _pos){
	return getImage(getFilenameFromPos(_pos));
}

ofImage imageStore::getImage(string _filename){
	ofImage retImage;
	if (!_filename.empty()){
		retImage.loadImage(folderPath + "/" + _filename);
	}
	return retImage;
}

ofImage imageStore::getLastImage(){
	return getImage(getImageCount());
}

bool imageStore::deleteImage(int _pos){
	return deleteImage(getFilenameFromPos(_pos));
}

bool imageStore::deleteImage(string _filename){
	if(imageExists(_filename)){
		string cmd = "rm "+ folderPath + "/" + _filename;
		int ret = system((const char *)cmd.c_str());
		cout<<"remove newest image "<<cmd<<endl;
		cout<<"return "<<ret<<endl;
		if (ret == 0){
			return true;
		}
	}
	return false;
}
