float t;


#include "testApp.h"

bool newImage = false;
#include "camera.h"


//--------------------------------------------------------------
void testApp::setup(){	 
	//ConnectCameras();

	//Load tracker
	tracker.setup();
	
	imageIndex = 0;
	
	//Setup imgStore
	imgStore.folderPath = "/Volumes/data/images";
	imgStore.ext = "jpg";
	lastImageCount = imgStore.getImageCount();

	//Load XML images
	bool fileLoaded = XML.loadFile("images.xml");
	if(fileLoaded){
		grid.loadXml(XML);
		nextPhotoDigit = grid.highestId+1; //The grid returns the highest ID in the xml file, which we use to define the next image.

		tracker.threshold = XML.getValue("TRACKER:THRESHOLD", 100);
		videoCamera.setup(XML);
	} 
	
	//Setup font
	font.loadFont("verdana.ttf", 10); 
	
	//Setup GUI
	showPoints = kofxGui_Button_Off;
	showTracker = kofxGui_Button_Off;
	capture = kofxGui_Button_Off;
	camDriver = kofxGui_Button_Off;
	mouseDriver = kofxGui_Button_On;

	gui	= ofxGui::Instance(this);

	ofxGuiPanel* panel1 = gui->addPanel(kParameter_Panel1, "properties", 10, 10, OFXGUI_PANEL_BORDER, OFXGUI_PANEL_SPACING);
	panel1->addButton(kParameter_ShowPoints, "Show Points", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, showPoints, kofxGui_Button_Switch);
	panel1->addButton(kParameter_ShowTracker, "Show Tracker", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, showTracker, kofxGui_Button_Switch);
	panel1->addButton(kParameter_Capture, "Take photos", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, capture, kofxGui_Button_Switch);
	panel1->addButton(kParameter_Capture, "Mouse driver", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, mouseDriver, kofxGui_Button_Switch);
	panel1->addButton(kParameter_Capture, "Camera driver", OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT, camDriver, kofxGui_Button_Switch);
	panel1->addSlider(kParameter_Threshold, "Threshold", 110, OFXGUI_SLIDER_HEIGHT, 0.0f, 300.0f, tracker.threshold, kofxGui_Display_Float2, 0);
	gui->forceUpdate(true);	
	gui->activate(true);
	
	
	photoCam.start();
	
	//Null init
	takingPhoto = 0;
	captureInterrupted = false;	
	captureCornerPoint = false;
	imageCaptured = false;
	blinkWhite = 0;	
	imageFileTimeout = 0;

}
//--------------------------------------------------------------
void testApp::update(){
	
	//Check if the grid should be expanded
	if(grid.numberEmptyPoints() < 15){
		grid.expandGrid();
	}
	
	//If capture is enabled
	if(capture){
		//If we in progress of taking a image, check if we have waited till end of delay
		/*if(takingPhoto != 0){
			string n;
			if(nextPhotoDigit< 10)
				n = "0000"+ofToString(nextPhotoDigit, 0);
			else if(nextPhotoDigit < 100)
				n = "000"+ofToString(nextPhotoDigit, 0);
			else if(nextPhotoDigit < 1000)
				n = "00"+ofToString(nextPhotoDigit, 0);
			else
				n = "0"+ofToString(nextPhotoDigit, 0);
			
			
			gridPoint* p = grid.findClosestPoint(marker.loc, GRIDPOINT_EMPTY);
			if(imageFileTimeout > 100){
				p->imageCaptured = false;
				p->loc.x = p->orig.x;
				p->loc.y = p->orig.y;
				p->empty = true;
				p->id = 0;
				imageFileTimeout = 0;
			} else if(captureInterrupted){
				marker.captureState = marker.CAPTURE_INTERRUPTED;
				marker.percent = ((float)ofGetElapsedTimeMillis() - takingPhoto)/PHOTODELAY;

				if(ofGetElapsedTimeMillis()-takingPhoto > PHOTODELAY){
					string cmd = "rm "+ofToDataPath("images/StopMotion1_"+n+".JPG");
					int ret = system((const char *)cmd.c_str());
					cout<<"remove newest image "<<cmd<<endl;
					cout<<"return "<<ret<<endl;
					if(ret == 0){
						marker.captureState = marker.CAPTURE_IDLE;
						captureInterrupted = false;
						takingPhoto = 0;
					}
				}
			}else if(ofGetElapsedTimeMillis()-takingPhoto > PHOTODELAY){				
				if(lastImageCount < imgStore.getImageCount()){
					takingPhoto = 0;
					imageCaptured = false;
					p->imageCaptured = true;
					p->url = "image/"+imgStore.getFilenameFromPos(imgStore.getImageCount());
					marker.captureState = marker.CAPTURE_IDLE;
					marker.percent = 0;
					lastImageCount = imgStore.getImageCount();
					p->savePoint(XML);

				} else {
					imageFileTimeout ++;
				}
				
			} else if(ofGetElapsedTimeMillis()-takingPhoto > PHOTORELEASEDELAY){
				marker.captureState = marker.CAPTURE_LOADING;
				marker.percent = ((float)ofGetElapsedTimeMillis() - (takingPhoto+ PHOTORELEASEDELAY))/(PHOTODELAY-PHOTORELEASEDELAY);
				if(!imageCaptured){
					//	takingPhoto = -100;
					p->loc.x = marker.loc.x;
					p->loc.y = marker.loc.y;
					
					p->empty = false;
					p->imageCaptured = false;
					p->id = nextPhotoDigit;
					p->url = "";

					nextPhotoDigit ++;
					imageCaptured = true;
					blinkWhite = 255;

				}
			} else {
				marker.captureState = marker.CAPTURE_CAPTURING;
				marker.percent = ((float)ofGetElapsedTimeMillis() - takingPhoto)/PHOTORELEASEDELAY;
				if(marker.loc.distance(grid.findClosestPoint(marker.loc, GRIDPOINT_EMPTY)->orig) > CAPTURERADIUS && !imageCaptured){
					captureInterrupted = true;
				}

			}
			
		//Check if we should capture image
		} else if(grid.findClosestPoint(marker.loc, GRIDPOINT_EMPTY) != NULL){ //Check if we even got any points
			if(marker.loc.distance(grid.findClosestPoint(marker.loc, GRIDPOINT_EMPTY)->orig) < CAPTURERADIUS){
				cout<<"Capture image"<<endl;
				capturePhoto();
				
				//Clean up the timeouter
				imageFileTimeout = 0;
				
				takingPhoto = ofGetElapsedTimeMillis();
			}
		} */
		
		switch (main_capture_state) {
			case MAIN_CAPTURE_READY:
				//CHeck if we should start capture a image
				if(grid.findClosestPoint(marker.loc, GRIDPOINT_EMPTY) != NULL){ //Check if we even got any points
					if(marker.loc.distance(grid.findClosestPoint(marker.loc, GRIDPOINT_EMPTY)->orig) < CAPTURERADIUS){
						cout<<"Capture image"<<endl;
						photoCam.takePicture();
						//capturePhoto();
		
						//Clean up the timeouter
						imageFileTimeout = 0;
						
						takingPhoto = ofGetElapsedTimeMillis();
					}
				}
				break;
			default:
				break;
		}
	}
	
	if(blinkWhite > 0){
		blinkWhite -= 10;
	}
	
	
	//Grabber stuff	
	tracker.update();
	
	if(tracker.pointMoved && camDriver){
		 marker.loc = tracker.getCurrentLocation(videoCamera);
	}
	
	i++;
	ofBackground(0, 0, 0);
	
	//Check if we need new image. Has to be done in update, and not mouse moved due bug
	loadX = (marker.loc.x);
	loadY = (marker.loc.y);
	if(loadedX != loadX || loadedY != loadY){
		loadImg(loadX,loadY);
	}

	
	//Update fade of images
	int totalAlpha = 0;
	for(int i=0;i<numImages;i++){
		if(i != imageIndex && imageAlpha[i] > 0) {
			imageAlpha[i] -= 20; 
			if(imageAlpha[i] < 0)
				imageAlpha[i] = 0;
			totalAlpha += pow(imageAlpha[i], GAMMA); 
		}
	}
	imageAlpha[imageIndex] = pow(pow(255,GAMMA)-totalAlpha,1/GAMMA);


	//Video camera calibration
	videoCamera.update(tracker.getCurrentLocation(videoCamera), captureCornerPoint);	
	if(captureCornerPoint){
		captureCornerPoint = false;
		if(videoCamera.state == videoCamera.STATE_MATRIX_DONE){
			XML.setValue("CALIBRATION:NE:X", videoCamera.dstPoints[0].x);
			XML.setValue("CALIBRATION:NE:Y", videoCamera.dstPoints[0].y);
			XML.setValue("CALIBRATION:SE:X", videoCamera.dstPoints[1].x);
			XML.setValue("CALIBRATION:SE:Y", videoCamera.dstPoints[1].y);
			XML.setValue("CALIBRATION:SW:X", videoCamera.dstPoints[2].x);
			XML.setValue("CALIBRATION:SW:Y", videoCamera.dstPoints[2].y);
			XML.setValue("CALIBRATION:NW:X", videoCamera.dstPoints[3].x);
			XML.setValue("CALIBRATION:NW:Y", videoCamera.dstPoints[3].y);
			XML.saveFile("images.xml");
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetupScreen();
	ofEnableAlphaBlending();

	glPushMatrix();
	glTranslated(ofGetWidth(), 0, 0);
	glRotated(90, 0, 0, 1);
	
	for(int i=0;i<numImages;i++){

		ofSetColor(255,255,255,imageAlpha[i]);
	
		images[i].draw(0,0, ofGetHeight(), ofGetWidth());
	}	
	glPopMatrix();
	
	
	if(showTracker){
		tracker.draw();
	}
	
	//Infobar
	/*ofSetColor(255, 255, 255);
	ofRect(0,0,ofGetWidth(), 15);
	ofSetColor(0, 0, 0);
	font.drawString(infoString, 0,10);*/

	//Lets stop the blending!
		
	//Draw points if settings says so
//	cout<<"Number points "<<grid.points.size()<<endl;

	
	//Draw debug for points
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
			ofCircle((float)grid.points[i].loc.x * (float)ofGetWidth(), (float)grid.points[i].loc.y*ofGetWidth(), 5);
		}
	}

	
	//Draw marker
	marker.draw();
	
	//Draw grid
	for(int i=0; i<grid.points.size(); i++){
		grid.points[i].draw(marker.loc);
	}
	

	ofDisableAlphaBlending();
	
	//For calibration
	videoCamera.draw(marker.loc);	

	
	gui->draw();

	//White capture blink
	ofEnableAlphaBlending();
	if(blinkWhite > 0){
		ofFill();
		ofSetColor(255, 255, 255, blinkWhite);
		ofRect(0,0,ofGetWidth(), ofGetHeight());
	}

}



void testApp::loadImg(float xin, float yin){
	if(grid.findClosestPoint(ofxPoint2f(xin,yin), GRIDPOINT_FULL)!= NULL){
		gridPoint newP = *grid.findClosestPoint(ofxPoint2f(xin, yin), GRIDPOINT_FULL);
		if(takingPhoto == 0 || captureInterrupted){
			if(newP.id != curId){
				//Load the image
				//	printf("loading %s\n", uri); 
			//	cout<<"Uri:"<<uri<<endl;
			//	infoString = uri;
				curId = newP.id;
				imageIndex = nextIndex();
				imageAlpha[imageIndex] = 0;
				t = ofGetElapsedTimeMillis();

				images[imageIndex].loadImage("/Volumes/data/"+newP.url);
			//	cout<<ofGetElapsedTimeMillis()-t<<endl;

				imageId[imageIndex] = curId;
				loadedX = xin;
				loadedY = yin;
			} 
		} else { 
			int a = 1;
		}
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if(key =='h'){
		photoCam.takePicture("");	
	}
	if(key == 'f'){
		ofToggleFullscreen();
	}
	if(key == 'g') 
		gui->activate(!gui->mIsActive);
	else if(key == 'c'){
		captureCornerPoint = true;
		
	}
	else if(key == 'p'){
/*	OSErr					err = noErr;
	char					*buf = NULL;
    PTPPassThroughPB 		*passThroughPB;
	ICAObjectSendMessagePB	msgPB;
	if (curDevice)
    {
        // call PTP command GetDeviceInfo via the pass through api
        buf =(char *) malloc( 64*1024 + sizeof(PTPPassThroughPB) );		// allocate enough buffer for the pass throught command
        if ( buf || 1 )
        {
			printf("Blag");
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
            
            err = ICAObjectSendMessage( &msgPB, NULL );

			/*passThroughPB = (PTPPassThroughPB *)buf;
            memset( buf, 0, 64*1024 + sizeof(PTPPassThroughPB) );
            passThroughPB->commandCode = 0xc009;
            passThroughPB->numOfInputParams = 0;
            passThroughPB->numOfOutputParams = 0;
            passThroughPB->dataUsageMode = kPTPPassThruReceive;
            passThroughPB->dataSize = 64*1024;*//*
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
	}*/
		}
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
//	if(i%1==0)
//		loadImg((y*1.0)/ofGetHeight(),1-(x*1.0)/ofGetWidth());
	if(mouseDriver){
		float Y = (float)y/ofGetHeight();
		float X = (float)x/ofGetWidth();
		cout<<X<<" "<<Y<<endl;
		marker.loc.x = X;
		marker.loc.y=  Y*ASPECTRATIO;
	}
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
		case kParameter_Capture:
			if(task == kofxGui_Set_Bool)
				capture = *(bool*)data;
			break;		
		case kParameter_Mousedriver:
			if(task == kofxGui_Set_Bool)
				mouseDriver = *(bool*)data;
			break;		
		case kParameter_Camdriver:
			if(task == kofxGui_Set_Bool)
				camDriver = *(bool*)data;
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

