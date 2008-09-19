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

}

int gPhotoCam::initCamera(){
	
}

int gPhotoCam::takePicture(string picturePath){
	if( lock() ){
		
		/*
		 * now in main... infp will be the stdin (in file descriptor)
		 * and outfp will be the stdout (out file descriptor)
		 * have fun
		 */
		
		memset (buf, 0x0, sizeof(buf));	
		write(infp, ("capture-image\n"), 14);
	//	close(infp);
		read(outfp, buf, 1024);
		printf("buf = '%s'\n", buf);
		
		unlock();
	}
	
}

void gPhotoCam::start(){
	startThread(true, false);   // blocking, verbose
	if (popen2("/opt/local/bin/gphoto2 --shell", &infp, &outfp) <= 0)
	{
		printf("Unable to exec gphoto2\n");
		exit(1);
	}
	
}

void gPhotoCam::stop(){
	write(infp, "q\n", 14);
	close(infp);

	stopThread(); 
}



void gPhotoCam::threadedFunction(){
	
/*	while( isThreadRunning() != 0 ){
		if( lock() ){
*/
			/*
			 * now in main... infp will be the stdin (in file descriptor)
			 * and outfp will be the stdout (out file descriptor)
			 * have fun
			 */
			
				
/*			unlock();
		}
	}*/
}
	
