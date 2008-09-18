#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#include "threadedObject.h"


#define OF_ADDON_USING_OFXXMLSETTINGS
#define OF_ADDON_USING_OFXVECTORMATH
#define OF_ADDON_USING_OFXOPENCV


#define ASPECTRATIO 3.0/2.0
#define GAMMA 1.801
#define PHOTODELAY 6500
#define PHOTORELEASEDELAY 4000
#define CAPTURERADIUS 0.03
#define MAXSHOWDIST 0.3



#define IMAGEPATH "/images"

#include "ofxGui.h"
#include "ofxGuiTypes.h"

#include "ofAddons.h"
#include "PTP.h"
#include "grid.h"
#include "tracker.h"
#include "videoCamera.h"
#include "marker.h"
#include "imageStore.h"


class testApp : public ofSimpleApp, public ofxGuiListener	{
	enum{
		kParameter_Panel1,
		
		kParameter_ShowPoints,
		kParameter_ShowTracker,
		kParameter_Capture,
		kParameter_Mousedriver,
		kParameter_Camdriver,
		kParameter_Threshold,
	};
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
   		void mousePressed(int x, int y, int button);
		void mouseReleased();
	
		//GUI Stuff
		void handleGui(int parameterId, int task, void* data, int length);

	
		void loadImg(float xin, float yin);
		void loadImage(string path);
	
		void capturePhoto();
	
		int curId;

		
		ofxXmlSettings XML;
		ofTrueTypeFont	font;

		string infoString;
		ofxGui*		gui;	
		bool showPoints;	
		bool showTracker;
		bool capture;
		bool mouseDriver;
		bool camDriver;
		
		static const int numImages = 20;
		ofImage images[numImages];
		int imageAlpha[numImages];
		int imageId[numImages];
		int imageIndex;
	
	int nextPhotoDigit;
	
	int takingPhoto;
	bool captureInterrupted;
	float blinkWhite;
	bool imageCaptured;
	
	ofxStopmotionMarker marker;
	
		int nextIndex(){
			int rimageIndex= imageIndex+1;
			if(rimageIndex>numImages-1){
				rimageIndex = 0;
			}
			return rimageIndex;
		}
		int prevIndex(){
			int rimageIndex= imageIndex-1;
			if(rimageIndex<0){
				rimageIndex = numImages-1;
			}
			return rimageIndex;
		}
	
		bool imageLoading;
		int i;
		double loadX, loadY, loadedX, loadedY;
	
		ofxGrid grid;
	
		//Tracking
		ofxTracker tracker;
	
	threadedObject	TO;
	
	
	imageStore imgStore;
	int lastImageCount;
	int imageFileTimeout;
	
	ofxVideoCamera videoCamera;
	bool captureCornerPoint;

};







#endif
	
