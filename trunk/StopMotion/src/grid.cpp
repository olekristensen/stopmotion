#include "testApp.h"



gridPoint::gridPoint(){
	empty = true;
}

ofxGrid::ofxGrid(){
}

void ofxGrid::loadXml(ofxXmlSettings &xmlFile){

	xmlFile.pushTag("IMAG", 0);
	int numTags = xmlFile.getNumTags("IMAGES:ID"); //Number of image tags saved in XML file
	points.reserve(numTags+points.size());//make space to all the images in points


	for(int i=0; i<numTags ; i++){
		xmlFile.pushTag("IMAGES", i);
		gridPoint p = gridPoint();
		p.loc.x = xmlFile.getValue("X",0.0,0);
		p.loc.y = xmlFile.getValue("Y",0.0,0);
		p.empty = false;
		p.id = xmlFile.getValue("ID",0,0);
		p.url = xmlFile.getValue("URI", "",0);
		xmlFile.popTag();
		points.push_back(p);
	}	
	xmlFile.popTag();
}

gridPoint ofxGrid::findClosestPoint(ofxPoint2f pos, int rule){
	gridPoint closestPoint;
	bool pointFound = false;
	float lowestDist;
	for(int i =0; i < points.size(); i++){
		if((rule == GRIDPOINT_EMPTY && points[i].empty == true) || (rule == GRIDPOINT_FULL && points[i].empty == false)){
			float newDist = points[i].loc.distanceSquared(pos);
			if(newDist < lowestDist || !pointFound){
				//This point is closer to pos
				lowestDist = newDist;
				closestPoint = points[i];
				pointFound = true;
			}
		}
	}	
//	if(pointFound){
		return closestPoint;
//	} else {
//		return ;
//	}
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

