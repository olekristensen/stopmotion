#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

#define HOST "ole-kristensens-powerbook.local"
#define PORT 12345

#include "ofMain.h"
#define OF_ADDON_USING_OFXTHREAD
#define OF_ADDON_USING_OFXOSC

#include "ofAddons.h"

// this is not a very exciting example yet 
// but ofThread provides the basis for ofNetwork and other 
// operations that require threading.
//
// please be careful - threading problems are notoriously hard 
// to debug and working with threads can be quite difficult


class threadedObject : public ofxThread{
	
public:
	ofxOscSender sender;

	
	int count;  // threaded fucntions that share data need to use lock (mutex)
	// and unlock in order to write to that data
	// otherwise it's possible to get crashes.
	//
	// also no opengl specific stuff will work in a thread...
	// threads can't create textures, or draw stuff on the screen
	// since opengl is single thread safe
	
	//--------------------------
	threadedObject(){
		//Setup OSC
		//sender.setup( HOST, PORT );
		
	
	}
	
	void start(){
		startThread(true, false);   // blocking, verbose
	}
	
	void stop(){
		stopThread(); 
	}
	
	//--------------------------
	void threadedFunction(){
		
		while( isThreadRunning() != 0 ){
			if( lock() ){
				system("osascript -e 'tell application \"RemoteCapture DC\" to activate' -e 'tell application \"System Events\" to tell process \"RemoteCapture DC\"' -e 'keystroke \"r\" using command down' -e 'end tell' ");	
/**				ofxOscMessage m;
				m.setAddress( "/capture/image" );
				m.addStringArg( "go" );
				sender.sendMessage( m );	
**/				
				unlock();
				stop();
			}
		}
	}
	
	
};

#endif
