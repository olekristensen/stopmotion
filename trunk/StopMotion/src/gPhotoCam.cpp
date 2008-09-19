/*
 *  gPhotoCam.cpp
 *  StopMotion2
 *
 *  Created by ole kristensen on 18/09/08.
 *  Copyright 2008 tabla.dk. All rights reserved.
 *
 */

#include "testApp.h"

pid_t popen2(const char *command, int *infp, int *outfp)
{
	int p_stdin[2], p_stdout[2];
	pid_t pid;
	
	if (pipe(p_stdin) != 0 || pipe(p_stdout) != 0)
		return -1;
	
	pid = fork();
	if (pid < 0)
		return pid;
	else if (pid == 0)
	{
		close(p_stdin[WRITE]);
		dup2(p_stdin[READ], READ);
		close(p_stdout[READ]);
		dup2(p_stdout[WRITE], WRITE);
		execl("/bin/sh", "sh", "-c", command, NULL);
		perror("execl");
		exit(1);
	}
	
	if (infp == NULL)
		close(p_stdin[WRITE]);
	else
		*infp = p_stdin[WRITE];
	if (outfp == NULL)
		close(p_stdout[READ]);
	else {
		*outfp = p_stdout[READ];
	}
	return pid;
}


gPhotoCam::gPhotoCam(){
	state = BUSY;
	hasCded = false;
}

int gPhotoCam::initCamera(){
	
}

string gPhotoCam::filenameOfLastPicture(){
	return filename;
}


int gPhotoCam::getState(){
	return state;
}

void gPhotoCam::setState(int _state){
	state = _state;
}

int gPhotoCam::takePicture(){
	if(state == READY)
		state=START_CAPTURE;
}

void gPhotoCam::start(){
	startThread(true, false);   // blocking, verbose
	if (popen2("/opt/local/bin/gphoto2 --shell", &infp, &outfp) <= 0)
	{
		printf("Unable to exec gphoto2\n");
		exit(1);
	}
	memset (buf, 0x0, 10000);	
	write(infp, ("lcd " + ofToDataPath("images/scaler") + "\n").c_str(), ("lcd " + ofToDataPath("images") + "\n").length());
	write(infp, ("capture-image\n"), 14);
	ofSleepMillis(30*100);
	read(outfp, buf, 10000);
	cout<<buf<<endl;
	state = READY;

}

void gPhotoCam::stop(){
	write(infp, "q\n", 14);
	close(infp);

	stopThread(); 
}

void gPhotoCam::threadedFunction(){
	
	while( isThreadRunning() != 0 ){

			if( lock() ){
				switch (state) {
					case START_CAPTURE:
						state = CAPTURING;
						/*
						 * now in main... infp will be the stdin (in file descriptor)
						 * and outfp will be the stdout (out file descriptor)
						 * have fun
						 */
						
						write(infp, ("cd /\n"), 5);
						ofSleepMillis(100);
						read(outfp, buf, 10000);
						
						write(infp, ("capture-image\n"), 14);
//						read(outfp, buf, 10000);
						ofSleepMillis(2000);
						milliseconds = ofGetElapsedTimeMillis();						
						
						memset (buf, 0x0, 10000);
						read(outfp, buf, 10000);
						bufStr = buf;
						//if(bufStr.substr(0, 7).compare("capture")){
						   pathname = bufStr.substr(48, 29);
						   filename = bufStr.substr(78, 12);
						/*} else {
						   cout<<"trick"<<endl;
						   pathname = bufStr.substr(48-28, 29);
						   filename = bufStr.substr(78-28, 12);
						}*/
						break;
					case CAPTURING:
//						if (ofGetElapsedTimeMillis() - milliseconds > 2*1000){
							state = CAPTURE_COMPLETE;
//						}
						break;
					case CAPTURE_COMPLETE:
						break;
					case START_DOWNLOAD:
						cout<<"=============FILENAMES:"<<endl<<buf<<endl<<"=="<<endl;
						cout<<"====Pathame: "<<pathname<<"==="<<endl;
						cout<<"===Filename: "<<filename<<"==="<<endl;
						memset (buf, 0x0, 10000);	
						//if(!hasCded){
						write(infp, ("cd /\n"), 5);
						ofSleepMillis(100);
						read(outfp, buf, 10000);
						
						write(infp, ("cd " + pathname + "\n").c_str(), 34);
						ofSleepMillis(100);
						read(outfp, buf, 10000);
						//} 
						
						write(infp, ("get " + filename + "\n").c_str(), 17);
						ofSleepMillis(100);
						read(outfp, buf, 10000);
						ofSleepMillis(100);
						cout<<"=============DOWNLOAD IMAGE:"<<endl<<buf<<endl<<"=="<<endl;;
						state = DOWNLOAD_COMPLETE;
						break;
					case INTERRUPTED:
			//			read(outfp, buf, 10000);
			//			cout<<buf<<endl;
						state = READY;
						break;
					case DOWNLOAD_COMPLETE:
						
						break;
					case READY:
						ofSleepMillis(100);
						break;
					default:
						break;
				}
				unlock();
			}
			
	}
}
	
