/*
 *  camera.h
 *  StopMotion2
 *
 *  Created by Jonas Jongejan on 14/08/08.
 *  Copyright 2008. All rights reserved.
 *
 */


ICAObject				deviceList;
ICAObject				curDevice;

void gotChildCount(ICAHeader* pb);
void GetChildCount(ICAObject icaObject);
void GetNthChild(ICAObject  parentObject,
                 UInt32		index,
                 UInt32		max);

ICAObject				gFirstImage = 0;
// ------------------------------------------------------------------------------------------------
//	gotImageData
// ------------------------------------------------------------------------------------------------
void gotImageData(ICAHeader* pb)
{
    ICAGetPropertyDataPB*	gotPropertyDataPB = (ICAGetPropertyDataPB*)pb;
	
	
	//
	//    if (gotPropertyDataPB->actualSize)
	//    {
	//        char*	path = "/Users/jonas/udvikling/of_preRelease_v0.05_xcode_FAT/apps/jonas/stopMotion2/data/ICASample.jpg";
	//        FILE * 	fi = fopen(path, "w");
	//		
	//        fwrite((Ptr)gotPropertyDataPB->dataPtr, sizeof(UInt8), gotPropertyDataPB->actualSize, fi);
	//        fclose(fi);
	//        free(gotPropertyDataPB->dataPtr);
	//		
	//        printf("image was downloaded ('%s')\n", path);
	//		
	//		newImage = true;
	//       // ImageWasDownloaded(gControllerRefCon, path);
	//    }
}

// ------------------------------------------------------------------------------------------------
//	gotImageDataProperty
// ------------------------------------------------------------------------------------------------
void gotImageDataProperty(ICAHeader* pb)
{
    ICAGetPropertyByTypePB* propertyByType 	  = (ICAGetPropertyByTypePB*)pb;
    ICAProperty 			imageDataProperty = propertyByType->property;
	
    printf("imageDataProperty = %08lX\n", (UInt32)imageDataProperty);
	
    if (imageDataProperty)
    {
        ICAGetPropertyDataPB	getPropertyDataPB;
        OSErr					err;
        void* 					imageDataPtr;
		
        imageDataPtr = malloc(propertyByType->propertyInfo.dataSize);
        memset (&getPropertyDataPB, 0, sizeof(ICAGetPropertyDataPB));
        getPropertyDataPB.property 	    = imageDataProperty;
        getPropertyDataPB.startByte	    = 0;
        getPropertyDataPB.requestedSize = propertyByType->propertyInfo.dataSize;
        getPropertyDataPB.dataPtr 		= imageDataPtr;
        err = ICAGetPropertyData(&getPropertyDataPB, gotImageData);
    }
}

void downloadFile(ICAObject object){
	OSErr err;
	FSRef pictFolderFSRef;
	FSRef fileFSRef;
	
	err = FSFindFolder(kOnAppropriateDisk, kCurrentUserFolderType, false, &pictFolderFSRef);
	if (noErr == err)
	{
		ICADownloadFilePB pb = {};
		pb.object   = object;
		pb.dirFSRef = &pictFolderFSRef;
		pb.flags    = kDeleteAfterDownload;
		pb.fileFSRef= &fileFSRef;
		err = ICADownloadFile(&pb, NULL);
		
		if (noErr == err)
		{
			char 	  fileName[255];
			
			printf("JAA");
			
			FSRefMakePath(&fileFSRef, (UInt8*)fileName, 255);
			char cmdBuff[200];
			sprintf(cmdBuff,"mv %s %s",fileName,"/Users/jonas/udvikling/of_preRelease_v0.05_xcode_FAT/apps/jonas/stopMotion2/data/image.jpg");
			system(cmdBuff);
			cout<<cmdBuff<<endl;
			newImage = true;
			
		} else if (dupFNErr == err)
		{
			printf("NEJ");
			
		} else
		{
			printf("NEJ");
		}
	} else {
		cout<<err<<endl;
	}
	
}

// ------------------------------------------------------------------------------------------------
//	gotNthChild
// ------------------------------------------------------------------------------------------------
void gotNthChild(ICAHeader* pb)
{
    ICAGetNthChildPB* 	getNthChildPB  = (ICAGetNthChildPB*)pb;
    ICAObject 			child 			= getNthChildPB->childObject;
	
	
    if (child)
    {
        if (getNthChildPB->childInfo.objectSubtype == kICAFileImage)
        {
            ICAGetPropertyByTypePB 	getPropertyByTypePB;
            OSErr					err;
			
            printf("got image = %08lX\n", (UInt32)child);
            if (gFirstImage == 0)
            {
                gFirstImage = child;
                printf("found first image: %08lX\n", (UInt32)gFirstImage);
                memset (&getPropertyByTypePB, 0, sizeof(ICAGetPropertyByTypePB));
				//                getPropertyByTypePB.header.refcon 	= pb->refcon;
				downloadFile(child);
				// getPropertyByTypePB.object 	   		= child;
				// getPropertyByTypePB.propertyType  	= kICAPropertyImageData;
				// err = ICAGetPropertyByType(&getPropertyByTypePB, gotImageDataProperty);
            }
        } else
        {
            if (getNthChildPB->header.refcon - 1 > getNthChildPB->index)
            {
                GetNthChild(getNthChildPB->parentObject, getNthChildPB->index+1, getNthChildPB->header.refcon);
            } else
            {
                if (getNthChildPB->childInfo.objectType == kICADirectory)
                {
                    printf("got directory = %08lX\n", (UInt32)child);
                    GetChildCount(child);
                } else if (getNthChildPB->childInfo.objectType == kICADevice)
                {
                    printf("got device = %08lX\n", (UInt32)child);
                    GetChildCount(child);
                }
            }
        }
    } else
    {
        printf("child = 0x00000000\n");
    }
}

// ------------------------------------------------------------------------------------------------
//	GetNthChild
// ------------------------------------------------------------------------------------------------
void GetNthChild(ICAObject  parentObject,
                 UInt32		index,
                 UInt32		max)
{
    ICAGetNthChildPB 	getNthChildPB;
    OSErr				err;
    
    memset(&getNthChildPB, 0, sizeof(ICAGetNthChildPB));
    getNthChildPB.header.refcon = max;
    getNthChildPB.parentObject  = parentObject;
    getNthChildPB.index         = index;
    err = ICAGetNthChild(&getNthChildPB, gotNthChild);
}

// ------------------------------------------------------------------------------------------------
//	gotChildCount
// ------------------------------------------------------------------------------------------------
void gotChildCount(ICAHeader* pb)
{
    ICAGetChildCountPB* childCountPB	= (ICAGetChildCountPB*)pb;
    ICAObject 			icaObject		= childCountPB->object;
    UInt32	  			count			= childCountPB->count;
	
    printf("child count for %08lX = %ld\n", (UInt32)icaObject, count);
	
    if (count > 0)
    {
        GetNthChild(icaObject, 0, count);
    }
}

// ------------------------------------------------------------------------------------------------
//	GetChildCount
// ------------------------------------------------------------------------------------------------
void GetChildCount(ICAObject icaObject)
{
    ICAGetChildCountPB 	getChildCountPB;
    OSErr				err;
	
    memset (&getChildCountPB, 0, sizeof(ICAGetChildCountPB));
    getChildCountPB.object	= icaObject;
    err = ICAGetChildCount(&getChildCountPB, gotChildCount);
}



void ConnectCameras(){
	OSErr				err;
 	ICAGetDeviceListPB	getDeviceListPB;
    
    printf("Rescanning for devices...\n");
	deviceList = NULL;
	bzero( &getDeviceListPB, sizeof(ICAGetDeviceListPB) );
	//	getDeviceListPB.header.refcon = (UInt32)self;
	err = ICAGetDeviceList (&getDeviceListPB, NULL);
	deviceList = getDeviceListPB.object;
    
    if ( deviceList )
    {
        ICAGetChildCountPB getChildCountPB;
        
        bzero( &getChildCountPB, sizeof(ICAGetChildCountPB) );
		//     getChildCountPB.header.refcon = (UInt32)self;
        getChildCountPB.object 	      = deviceList;
        err = ICAGetChildCount( &getChildCountPB, NULL );
        
        if ( getChildCountPB.count == 0 )
        {
            curDevice = NULL;
            printf("No device found!\n");
        }
		else
        {
			// We are assuming that a PTP type curDevice is the only curDevice
			// on the bus and pick the first child to control.
			// You may want to modify this part to iterate over the device list
			// and pick a device of interest.
			
            ICAGetNthChildPB getNthChildPB;
            
            bzero( &getNthChildPB, sizeof(ICAGetNthChildPB) );
			//   getNthChildPB.header.refcon = (UInt32)self;
            getNthChildPB.parentObject  = getChildCountPB.object;
            getNthChildPB.index         = 0;
            err = ICAGetNthChild( &getNthChildPB, NULL );
            if ( getNthChildPB.childObject )
            {
                curDevice = getNthChildPB.childObject;
                printf("Found a device.\n");
            }
        }
    }
	
}


void photoTaken(ICAHeader* pb){
	time_t start_time, cur_time;
	time(&cur_time);
	time(&start_time);
	do
	{
		time(&cur_time);
	}
	while((cur_time - start_time) < 1);
	
	GetChildCount(curDevice);
}