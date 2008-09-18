//#include "testApp.h"


class gridPoint : public ofSimpleApp{
public:
	bool empty;
	ofxPoint2f loc;
	ofxPoint2f orig;  
	string url;
	int id;

	gridPoint();
	void savePoint(ofxXmlSettings &xmlFile);
	void draw(ofxPoint2f tracker);
	
	float size;
	float sizeV;
	float capturePercent;
	bool imageCaptured;
};


#define GRIDPOINT_ALL 1 
#define GRIDPOINT_EMPTY 2
#define GRIDPOINT_FULL 3

class ofxGrid : public ofSimpleApp{
public:
	ofxGrid();
	void loadXml(ofxXmlSettings &xmlFile);
	void addPoint();
	void expandGrid();
	void makeGrid();
	gridPoint* findClosestPoint(ofxPoint2f pos, int rule = GRIDPOINT_ALL);
	int numberEmptyPoints();
	
	vector<gridPoint> points;//Contains all points
	int gridSize;
	
	ofxXmlSettings XML;
	
	int highestId;
};

