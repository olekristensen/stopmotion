#include "testApp.h"



gridPoint::gridPoint(){
	empty = true;
	capturePercent = 0;
	imageCaptured = true;
}

void gridPoint::savePoint(ofxXmlSettings &xmlFile){
	xmlFile.pushTag("IMAG", 0);
	int numTags = xmlFile.getNumTags("IMAGES:ID"); //Number of image tags saved in XML file
	bool tagFound = false;
	for(int i=0; i<numTags ; i++){
		xmlFile.pushTag("IMAGES", i);
		if(xmlFile.getValue("ID",-1,0) == id){
			tagFound = true; 
			xmlFile.popTag();
			break;
		}
		xmlFile.popTag();
	} 
	xmlFile.popTag();
	xmlFile.popTag();
	xmlFile.pushTag("IMAG", 0);

	if(!tagFound){
		numTags = xmlFile.addTag("IMAGES");
	}
	xmlFile.pushTag("IMAG", 0);
	xmlFile.pushTag("IMAGES", numTags);

	xmlFile.setValue("X", loc.x);
	xmlFile.setValue("Y", loc.y);
	xmlFile.setValue("ORIGX", orig.x);
	xmlFile.setValue("ORIGY", orig.y);
	xmlFile.setValue("ID", id);
	xmlFile.setValue("URI", url);
	xmlFile.popTag();
	xmlFile.popTag();

	xmlFile.saveFile("images.xml");
}

void gridPoint::draw(ofxPoint2f tracker){

//	glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  //  glLineWidth(3);

	if(capturePercent > 0 && capturePercent < 1){
		sizeV += (CAPTURERADIUS - size)*0.01;
	} else if(capturePercent >=1  && size > 0){
		sizeV += (0 - size)*0.01;
	}else if(size >0){
		sizeV -= 0.01;
	} else {
		sizeV = 0;
		size = 0;
	}
	sizeV *= 0.9;
	size += sizeV;
	
	if(size >0){
		int resolution = 200;
		ofSetCircleResolution(resolution);
	/*	ofNoFill();
		ofSetColor(255, 255, 255);
		ofCircle(orig.x*ofGetWidth(), orig.y*ofGetWidth(), size*ofGetWidth());
		ofSetColor(200,200,200);
		ofCircle(orig.x*ofGetWidth(), orig.y*ofGetWidth(), size*ofGetWidth()-1);
		ofCircle(orig.x*ofGetWidth(), orig.y*ofGetWidth(), size*ofGetWidth()+1);
	*/	
		loc = tracker;
		int numBricks = round(resolution*capturePercent);
		double radianStep = (double)TWO_PI/resolution;
		glColor3f(0, 0, 0);
		glBegin(GL_TRIANGLES);
//		glVertex2f(orig.x*ofGetWidth(), orig.y*ofGetWidth());
		for(int i=-resolution/4;i<numBricks-resolution/4; i++){
			glVertex2f(loc.x*ofGetWidth(), loc.y*ofGetWidth());
			glVertex2f(loc.x*ofGetWidth()+(CAPTURERADIUS)*cos(i*radianStep)*ofGetWidth(), loc.y*ofGetWidth()+(CAPTURERADIUS)*sin(i*radianStep)*ofGetWidth());
			glVertex2f(loc.x*ofGetWidth()+(CAPTURERADIUS)*cos((i-1)*radianStep)*ofGetWidth(), loc.y*ofGetWidth()+(CAPTURERADIUS)*sin((i-1)*radianStep)*ofGetWidth());
		}
//		glVertex2f(orig.x*ofGetWidth(), orig.y*ofGetWidth());
		glEnd();
		/*
		glColor3f(100, 100, 100);
		glBegin(GL_TRIANGLES);
		for(int i=0;i<numBricks; i++){
			glVertex2f(orig.x*ofGetWidth(), orig.y*ofGetWidth());
			glVertex2f(orig.x*ofGetWidth()+(CAPTURERADIUS-0.01)*cos(i*radianStep)*ofGetWidth(), orig.y*ofGetWidth()+(CAPTURERADIUS-0.01)*sin(i*radianStep)*ofGetWidth());
			glVertex2f(orig.x*ofGetWidth()+(CAPTURERADIUS-0.01)*cos((i-1)*radianStep)*ofGetWidth(), orig.y*ofGetWidth()+(CAPTURERADIUS-0.01)*sin((i-1)*radianStep)*ofGetWidth());
		}
		glEnd();
		*/
			
		
		
	}
	float dist = tracker.distance(orig);
	if (dist < MAXSHOWDIST && empty  ){
		ofFill();
		ofSetColor(255, 255, 255, 100.0-(float)(dist/MAXSHOWDIST)*100.0);
		ofCircle(orig.x*ofGetWidth(), orig.y*ofGetWidth(), 5);
	}
}

ofxGrid::ofxGrid(){
	gridSize = 0;
}

void ofxGrid::loadXml(ofxXmlSettings &xmlFile){
//	int size = xmlFile.getNumTags("GRID:SIZE");
	
	XML = xmlFile;
	xmlFile.pushTag("IMAG", 0);
	int numTags = xmlFile.getNumTags("IMAGES:ID"); //Number of image tags saved in XML file
	points.reserve(numTags+points.size());//make space to all the images in points


	for(int i=0; i<numTags ; i++){
		xmlFile.pushTag("IMAGES", i);
		gridPoint p = gridPoint();
		p.loc.x = xmlFile.getValue("X",0.0,0);
		p.loc.y = xmlFile.getValue("Y",0.0,0);
		p.orig.x = xmlFile.getValue("ORIGX",0.0,0);
		p.orig.y = xmlFile.getValue("ORIGY",0.0,0);
		p.empty = false;
		p.id = xmlFile.getValue("ID",0,0);
		p.url = xmlFile.getValue("URI", "",0);
		xmlFile.popTag();
		points.push_back(p);
		if(p.id > highestId)
			highestId = p.id;
	}	
	xmlFile.popTag();
	
	float x[50];
	int in = 0;
	for(int i=0;i<points.size();i++){
		for(int u=0; u<51; u++){
			if(x[u] == points[i].orig.x){
				break;
			} else if(in +1 == u){
				x[u] = points[i].orig.x;	
				in ++;
				break;
			}
		}
	}
	while(gridSize < in){
		gridSize = (gridSize * 2) + 1;
	}
	makeGrid();
	cout<<"Load grid size: "<<in<<endl;
}

void ofxGrid::expandGrid(){
	gridSize = (gridSize * 2) + 1;
	cout<<"Resize grid to "<<gridSize<<endl;
	makeGrid();
}

void ofxGrid::makeGrid(){
	cout<<"Make grid: "<<gridSize<<endl;
	
	float dist = 1.0/(gridSize+1);
	for(int i = 1; i<= gridSize; i++){
		for(int u = 1; u <= gridSize * ASPECTRATIO + 1; u++){
			bool pointExist = false;
			for(int p=points.size()-1; p>=0; p--){
				if(points[p].orig.x == dist*i && points[p].orig.y == dist*u){
					pointExist = true;
					break;
				}
			}
			if(!pointExist){
				gridPoint p = gridPoint();
				p.orig.x = p.loc.x = dist*i;
				p.orig.y = p.loc.y = dist*u;
				points.push_back(p);
				cout<<"Add empty point x: "<<p.loc.x<<" y: "<< p.loc.y <<endl;
			}
		}
	}
}

gridPoint* ofxGrid::findClosestPoint(ofxPoint2f pos, int rule){
	int closestPoint ;
	bool pointFound = false;
	float lowestDist = ASPECTRATIO*ASPECTRATIO;
	for(int i =0; i < points.size(); i++){
		if((rule == GRIDPOINT_EMPTY && points[i].empty == true) || (rule == GRIDPOINT_FULL && points[i].empty == false)){
			float newDist = points[i].loc.distanceSquared(pos);
			if(newDist < lowestDist || !pointFound){
				//This point is closer to pos
				lowestDist = newDist;
				closestPoint = i;
				pointFound = true;
			}
		}
	}	
	if(pointFound){
		return &points[closestPoint];
	} else {
		return NULL;
	}
}


int ofxGrid::numberEmptyPoints(){
	int n =0;
	for(int i=0; i<points.size(); i++){
		if(points[i].empty){
			n ++;
		}
	}
	return n;
}

