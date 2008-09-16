class ofxTracker : public ofSimpleApp {
public:
	
	void setup();
	void update();

	ofxPoint2f getCurrentLocation();
	
	ofVideoGrabber 		vidGrabber;
	
	ofxCvColorImage		colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	
	int 				threshold;
};