#include "videoCamera.h"

class ofxTracker : public ofSimpleApp {
public:
	
	void setup();
	void update(bool moveCursor);
	void draw();

	ofxPoint2f getCurrentLocation(ofxVideoCamera cam);
	
	ofxPoint2f loc;
	
	ofVideoGrabber 		vidGrabber;
	
	ofxCvColorImage		colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	
	int threshold;
	bool pointMoved;
	
	ofTrueTypeFont	font;

};