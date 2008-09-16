#include "testApp.h"



gridPoint::gridPoint(){
	empty = true;
}

ofxGrid::ofxGrid(){
	gridSize = 0;
}

void ofxGrid::loadXml(ofxXmlSettings &xmlFile){
//	int size = xmlFile.getNumTags("GRID:SIZE");
	
	
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
	gridSize = in;
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
			for(int p=points.size()-1; p>=0; p--){
				if(points[p].orig.x == dist*i && points[p].orig.y == dist*u){
					break;
				}
			}
			gridPoint p = gridPoint();
			p.orig.x = p.loc.x = dist*i;
			p.orig.y = p.loc.y = dist*u;
			points.push_back(p);
			cout<<"Add empty point x: "<<p.loc.x<<" y: "<< p.loc.y <<endl;
			
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

