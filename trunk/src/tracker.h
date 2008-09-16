class ofxTracker : public ofSimpleApp {
public:
	
	void setup();
	void update();
	void draw();

	ofxPoint2f getCurrentLocation();
	
	ofxPoint2f loc;
	
	ofVideoGrabber 		vidGrabber;
	
	ofxCvColorImage		colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	
	int 				threshold;
};