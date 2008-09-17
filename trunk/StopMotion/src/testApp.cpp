float t;


#include "testApp.h"

bool newImage = false;
#include "camera.h"


//--------------------------------------------------------------
void testApp::setup(){	 
	ConnectCameras();

	//Load tracker
	tracker.setup();
	
	imageIndex = 0;
	
	//Load XML images
	bool fileLoaded = XML.loadFile("images.xml");
	if(fileLoaded){
		grid.loadXml(XML);
		tracker.threshold = XML.getValue("TRACKER:THRESHOLD", 100);
		
		nextPhotoDigit = grid.highestId+1; //The grid returns the highest ID in the xml file, which we use to define the next image.
	} 
	
	//Setup font
	font.loadFont("verdana.ttf", 10); 
	
	//Setup GUI
	showPoints = kofxGui_Button_Off;
	showTracker = kofxGui_Button_Off;
	
	gui	= ofxGui::Instance(this);

	ofxGuiPanel* panel1 = gui->addPanel(kParameter_Panel1, "properties", 10, 10, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	panel1->addButton(kParameter_ShowPoints, "Show Points", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, showPoints, kofxGui_Button_Switch);
	panel1->addButton(kParameter_ShowTracker, "Show Tracker", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, showTracker, kofxGui_Button_Switch);
	panel1->addSlider(kParameter_Threshold, "Threshold", 110, OFXGUI_SLIDER_HEIGHT, 0.0f, 300.0f, tracker.threshold, kofxGui_Display_Float2, 0);
	gui->forceUpdate(true);	
	gui->activate(true);
	
	marker.loadImage("Marker.png");
	
	
	//Null init
	takingPhoto = 0;
	captureInterrupted = false;
	
	captureCornerPoint = false;
	
	videoCamera.setup();
}
//--------------------------------------------------------------
void testApp::update(){

	//Check if the grid should be expanded
	if(grid.numberEmptyPoints() < 5){
		grid.expandGrid();
	}

	//If we in progress of taking a image, check if we have waited till end of delay
	if(takingPhoto != 0){
		string n;
		if(nextPhotoDigit< 10)
			n = "000"+ofToString(nextPhotoDigit, 0);
		else if(nextPhotoDigit < 100)
			n = "00"+ofToString(nextPhotoDigit, 0);
		else if(nextPhotoDigit < 1000)
			n = "0"+ofToString(nextPhotoDigit, 0);
		else
			n = ofToString(nextPhotoDigit, 0);
		
		
		if(captureInterrupted){
			if(ofGetElapsedTimeMillis()-takingPhoto > PHOTODELAY){
				string cmd = "rm "+ofToDataPath("images/StopMotion1_"+n+".JPG");
				system((const char *)cmd.c_str());
				captureInterrupted = false;
				takingPhoto = 0;
			}
		}
		else if(tracker.getCurrentLocation().distance(grid.findClosestPoint(tracker.getCurrentLocation(), GRIDPOINT_EMPTY)->orig) > CAPTURERADIUS){
			//takingPhoto = 0;
			captureInterrupted = true;
			gridPoint* p = grid.findClosestPoint(tracker.getCurrentLocation(), GRIDPOINT_EMPTY);
			p->capturePercent = 0;
		} else {
			gridPoint* p = grid.findClosestPoint(tracker.getCurrentLocation(), GRIDPOINT_EMPTY);
			p->capturePercent = ((float)ofGetElapsedTimeMillis() - takingPhoto)/PHOTODELAY;
			if(ofGetElapsedTimeMillis()-takingPhoto > PHOTODELAY){
				p->capturePercent = 1;
				takingPhoto = 0;
				p->loc.x = tracker.getCurrentLocation().x;
				p->loc.y = tracker.getCurrentLocation().y;
								
				p->url = "images/StopMotion1_"+n+".JPG";
				p->empty = false;
				p->id = nextPhotoDigit;
				nextPhotoDigit ++;
				p->savePoint(XML);
			}
		}
		
	//Check if we should capture image
	} else if(grid.findClosestPoint(tracker.getCurrentLocation(), GRIDPOINT_EMPTY) != NULL){ //Check if we even got any points
		if(tracker.getCurrentLocation().distance(grid.findClosestPoint(tracker.getCurrentLocation(), GRIDPOINT_EMPTY)->orig) < CAPTURERADIUS){
			cout<<"Capture image"<<endl;
			capturePhoto();
			takingPhoto = ofGetElapsedTimeMillis();
		}
	}
	
	//Grabber stuff	
	tracker.update();
	
	if(tracker.pointMoved){
		ofxPoint2f loc = tracker.getCurrentLocation();
		loadX = (loc.x*1.0);
		loadY = (loc.y*1.0);
	}
	
	i++;
	ofBackground(0, 0, 0);
	//Check if we need new image. Has to be done in update, and not mouse moved due bug
	if(loadedX != loadX || loadedY != loadY){
		loadImg((loadX),(loadY));
	}

	
	//Update fade
	int totalAlpha = 0;
	for(int i=0;i<numImages;i++){
		if(i != imageIndex && imageAlpha[i] > 0) {
			imageAlpha[i] -= 20; 
			if(imageAlpha[i] < 0)
				imageAlpha[i] = 0;
			totalAlpha += pow(imageAlpha[i], GAMMA); 
		}
	}
	imageAlpha[imageIndex] = pow(pow(255,GAMMA)-totalAlpha,0.555);
	//cout<<totalAlpha+(255-totalAlpha)<<endl;

	videoCamera.update(tracker.getCurrentLocation(), captureCornerPoint);
	
	if(captureCornerPoint){
		captureCornerPoint = false;
	}
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetupScreen();
	ofEnableAlphaBlending();

	glPushMatrix();
	glPushMatrix();
	glTranslated(ofGetWidth(), 0, 0);
	glRotated(90, 0, 0, 1);
	
	for(int i=0;i<numImages;i++){

		ofSetColor(255,255,255,imageAlpha[i]);
	
		images[i].draw(0,0, ofGetHeight(), ofGetWidth());
	}	
	glPopMatrix();
	//Infobar
	/*ofSetColor(255, 255, 255);
	ofRect(0,0,ofGetWidth(), 15);
	ofSetColor(0, 0, 0);
	font.drawString(infoString, 0,10);*/

	//Lets stop the blending!
		
	//Draw points if settings says so
//	cout<<"Number points "<<grid.points.size()<<endl;
	for(int i=0; i<grid.points.size(); i++){
		grid.points[i].draw(tracker.getCurrentLocation());
	}
	
	if(showPoints){
		for(int i=0; i<grid.points.size(); i++){
			if(grid.points[i].empty)
			   ofNoFill();
			else
				ofFill();

			if(imageId[imageIndex] == grid.points[i].id){
				ofSetColor(255, 0, 0,90);
			} else {
				ofSetColor(0, 0, 255, 90);
			}
			ofCircle((float)grid.points[i].loc.x * (float)ofGetWidth(), (float)grid.points[i].loc.y*ofGetWidth(), 3);
		}
	}
	
	ofSetColor(255, 255, 255,60);
	float markerSize = CAPTURERADIUS*4*ofGetWidth();
	marker.draw(tracker.getCurrentLocation().x*ofGetWidth()-markerSize*0.5, tracker.getCurrentLocation().y*ofGetWidth()-markerSize*0.5,markerSize,markerSize);

	ofDisableAlphaBlending();
	
	if(showTracker){
		tracker.draw();
	}

	
	glPopMatrix();

	
	videoCamera.draw(tracker.getCurrentLocation());	

	
	gui->draw();

	
}



void testApp::loadImg(float xin, float yin){
	if(grid.findClosestPoint(ofxPoint2f(xin, yin), GRIDPOINT_FULL)!= NULL){
		gridPoint newP = *grid.findClosestPoint(ofxPoint2f(xin, yin), GRIDPOINT_FULL);

		if(newP.id != curId){
			//Load the image
			//	printf("loading %s\n", uri); 
		//	cout<<"Uri:"<<uri<<endl;
		//	infoString = uri;
			curId = newP.id;
			imageIndex = nextIndex();
			imageAlpha[imageIndex] = 0;
			t = ofGetElapsedTimeMillis();

			images[imageIndex].loadImage(newP.url);
			cout<<ofGetElapsedTimeMillis()-t<<endl;

			imageId[imageIndex] = curId;
			loadedX = xin;
			loadedY = yin;
		} 
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if(key == 'g') 
		gui->activate(!gui->mIsActive);
	else if(key == 'c'){
		captureCornerPoint = true;
	}else if(key == 'p'){
	OSErr					err = noErr;
	char					*buf = NULL;
    PTPPassThroughPB 		*passThroughPB;
	ICAObjectSendMessagePB	msgPB;
	if (curDevice)
    {
        // call PTP command GetDeviceInfo via the pass through api
        buf =(char *) malloc( 64*1024 + sizeof(PTPPassThroughPB) );		// allocate enough buffer for the pass throught command
        if ( buf || 1 )
        {
		/*	printf("Blag");
            passThroughPB = (PTPPassThroughPB *)buf;
            memset( buf, 0, 64*1024 + sizeof(PTPPassThroughPB) );
            passThroughPB->commandCode = 0x9008;
            passThroughPB->numOfInputParams = 0;
            passThroughPB->numOfOutputParams = 0;
            passThroughPB->dataUsageMode = kPTPPassThruReceive;
            passThroughPB->dataSize = 64*1024;
            memset( &msgPB, 0, sizeof( ICAObjectSendMessagePB ) );
            msgPB.object				= curDevice;
            msgPB.message.messageType	= 'pass';
            msgPB.message.startByte		= 0;
            msgPB.message.dataPtr		= buf;
            msgPB.message.dataSize		= 64*1024 + sizeof(PTPPassThroughPB);
            msgPB.message.dataType		= kICATypeData;
            
            err = ICAObjectSendMessage( &msgPB, NULL );
			
			passThroughPB = (PTPPassThroughPB *)buf;
            memset( buf, 0, 64*1024 + sizeof(PTPPassThroughPB) );
            passThroughPB->commandCode = 0x901a;
            passThroughPB->numOfInputParams = 0;
            passThroughPB->numOfOutputParams = 0;
            passThroughPB->dataUsageMode = kPTPPassThruReceive;
            passThroughPB->dataSize = 64*1024;
            memset( &msgPB, 0, sizeof( ICAObjectSendMessagePB ) );
            msgPB.object				= curDevice;
            msgPB.message.messageType	=  'pass';
            msgPB.message.startByte		= 0;
            msgPB.message.dataPtr		= buf;
            msgPB.message.dataSize		= 64*1024 + sizeof(PTPPassThroughPB);
            msgPB.message.dataType		= kICATypeData;
            
            err = ICAObjectSendMessage( &msgPB, NULL );*/

			/*passThroughPB = (PTPPassThroughPB *)buf;
            memset( buf, 0, 64*1024 + sizeof(PTPPassThroughPB) );
            passThroughPB->commandCode = 0xc009;
            passThroughPB->numOfInputParams = 0;
            passThroughPB->numOfOutputParams = 0;
            passThroughPB->dataUsageMode = kPTPPassThruReceive;
            passThroughPB->dataSize = 64*1024;*/
			ICAObjectSendMessagePB  pb = {};
            pb.object				=	curDevice;
            pb.message.messageType	=	kICAMessageCameraCaptureNewImage;            
            err = ICAObjectSendMessage( &pb, photoTaken );
			
			if (noErr != err)
			{
				// handle error
			} else
			{
				// pb.dataType     // OSType
				// pb.actualSize   // UInt32
				// pb.dataPtr      // void *
			}
			
			
			
		//	Logger("   params[0]:            0x%08x\n", passThroughPB->data );
            
            if ( noErr == err )
            {
				// print event data
				
        //        DumpData((Ptr)(passThroughPB->data), passThroughPB->dataSize);
            }
            free( buf );
        }
    } 
	}
	else {
		system("osascript -e 'tell application \"RemoteCapture DC\" to activate' -e 'tell application \"System Events\" to tell process \"RemoteCapture DC\"' -e 'keystroke \"r\" using command down' -e 'end tell' ");
	}
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
//	if(i%1==0)
//		loadImg((y*1.0)/ofGetHeight(),1-(x*1.0)/ofGetWidth());
	loadY = 1-(y*1.0)/ofGetHeight();
	loadX = (x*1.0)/ofGetWidth();
	tracker.loc.x = loadX*320;
	tracker.loc.y=  loadY*240;
}

//--------------------------------------------------------------

void testApp::mouseDragged(int x, int y, int button)
{
	gui->mouseDragged(x, y, button);	
}

//--------------------------------------------------------------

void testApp::mousePressed(int x, int y, int button)
{
	gui->mousePressed(x, y, button);	
}

//--------------------------------------------------------------

void testApp::mouseReleased()
{
	gui->mouseReleased(mouseX, mouseY, 0);	
}

//--------------------------------------------------------------

void testApp::handleGui(int parameterId, int task, void* data, int length){
	switch(parameterId){
		case kParameter_ShowPoints:
			if(task == kofxGui_Set_Bool)
				showPoints = *(bool*)data;
			break;		
		case kParameter_ShowTracker:
			if(task == kofxGui_Set_Bool)
				showTracker = *(bool*)data;
			break;		
		case kParameter_Threshold:
			if(task == kofxGui_Set_Float)
				tracker.threshold = *(float*)data;
				XML.setValue("TRACKER:THRESHOLD", tracker.threshold);
				XML.saveFile("images.xml");
			break;		
	}

}


void testApp::capturePhoto(){
//	system("osascript -e 'tell application \"RemoteCapture DC\" to activate' -e 'tell application \"System Events\" to tell process \"RemoteCapture DC\"' -e 'keystroke \"r\" using command down' -e 'end tell' ");	
	TO.start();
}

