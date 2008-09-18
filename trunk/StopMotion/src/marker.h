#ifndef _OFX_MARKER_H_
#define _OFX_MARKER_H_


class ofxStopmotionMarker : public ofSimpleApp {
public:
	ofxStopmotionMarker();
	ofxPoint2f loc;
	ofImage markerImg;
	float percent;
	enum { CAPTURE_IDLE, CAPTURE_CAPTURING, CAPTURE_INTERRUPTED, CAPTURE_LOADING };
	int captureState;
	
	void draw();
};

#endif