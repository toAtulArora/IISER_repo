#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

#include <iostream>
#include <iterator>
#include <cctype>
#include <vector>
#include <math.h>
#include <fstream>
// #include <fstream>
// #include <sstream>
#include "boost/filesystem.hpp"

#define PI 3.14159265
using namespace cv;
using namespace std;
void getMidPoint(Rect r,float& x, float& y)
{
	x=r.x + (r.width/2);
	y=r.y + (r.height/2);	
}

int main(int argc, const char *argv[])
{
	string windowName="Camera Stream",debugWindow="Gray-Scale",candidateWindow="Candidate Preview",directoryToUse="Atul";
	namedWindow(windowName,WINDOW_AUTOSIZE);
	namedWindow(debugWindow,WINDOW_AUTOSIZE);
	namedWindow(candidateWindow,WINDOW_AUTOSIZE);
	Mat frame,frameClone,gray,roi,visRoi,trainRoi,visTrainRoi;
	Mat rotMat(2,3,CV_32FC1);

	bool getCandidate=false,candidateInBuffer=false;
	
	//Settings
	bool waitForEyes=true;

	ifstream fileTest;

	string cascadeFn = "c:/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
	string nestedFn = "c:/opencv/data/haarcascades/haarcascade_eye.xml";
	//CascadeClassifier cascade(cascadeFn), nestedCascade(nestedFn);
	CascadeClassifier cascade,nestedCascade;
	if(!cascade.load(cascadeFn))
		cout<<"Couldn't open the cascade file"<<endl<<cascadeFn;
	if(!nestedCascade.load(nestedFn))
		cout<<"Couldn't open the cascade for the eyes"<<endl<<nestedFn;

	// CascadeClassifier(cascadeFn);
	
	// video source = 0 (default camera)
	VideoCapture capture(0);
	if(!capture.isOpened())
		cout<<"Couldn't read from the camera";
	
	for(;;)
	{
		capture>>frame;
		frameClone=frame.clone();
		if(frame.empty())
			cout<<"Can't read from the stream";					
		
		cvtColor(frame,gray,COLOR_BGR2GRAY);
		equalizeHist(gray,gray);
		vector<Rect> rects,subrects;
		
		// CascadeClassifier::detectMultiScale(const Mat& image, vector<Rect>& objects, double scaleFactor=1.1, int minNeighbors=3, int flags=0, Size minSize=Size(), Size maxSize=Size())		
		cascade.detectMultiScale(gray,rects,1.3,4,CV_HAAR_SCALE_IMAGE,Size(30,30),Size(300,300));		
		for(Rect r: rects)
		{
			roi=gray(r);
			nestedCascade.detectMultiScale(roi,subrects,1.3,4,CV_HAAR_SCALE_IMAGE,Size(r.width/5,r.height/5),Size(r.width/2.5,r.height/2.5));
			rectangle(frame,r,Scalar(255,0,0));

			//visRoi=frame(r);
			for(Rect rr: subrects)
				rectangle(frame(r),(rr),Scalar(0,255,0));
			
			
			if(getCandidate)
			{
				//This may be slightly unclear, but when you want to acquire another candidate, then 
				//save the previous one, if there was any
				//Another key can be used to reject
				if(candidateInBuffer)
				{
					static int count=0;					
					char filename[10];
					//string filename="";
					vector<int> compression_params;
					compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
					compression_params.push_back(9);

					//Ensure the directory corresponding to the label exists
					boost::filesystem::path rootPath ( "." );
					boost::filesystem::path labelPath (directoryToUse);
					boost::filesystem::path finalPath = rootPath/labelPath;
					boost::system::error_code returnedError;

					boost::filesystem::create_directories( finalPath, returnedError );

					if ( returnedError )
					   cout<<"Fudge! Something went wrong while trying to create the desired directory"; //did not successfully create directories
					else
					{
						//directories successfully created
						//Finds the best file name to use
						do
						{
							fileTest.close();
							count++;
							//filename<<finalPath.string()<<count;
							sprintf(filename,"%s/%d.png",(finalPath.string()).c_str(),count);
							cout<<filename<<endl;
							fileTest.open(filename);
						} while(fileTest.is_open());

						imwrite(filename,trainRoi,compression_params);
						cout<<"Image saved with name "<<count<<endl;
					}
					candidateInBuffer=false;	//Saved! (Hopefully that is, else move on)
				}
				if(!waitForEyes)
				{
					getCandidate=false;
					Point center=Point(frame(r).cols/2,frame(r).rows/2);
					rotMat=getRotationMatrix2D(center,0,1.15);
					warpAffine(frame(r),visTrainRoi,rotMat,frame(r).size());
					warpAffine(frameClone(r),trainRoi,rotMat,frame(r).size());
					imshow(candidateWindow,visTrainRoi);
					candidateInBuffer=true;
				}
				else if(subrects.size()==2 && waitForEyes==true)
				{
					getCandidate=false;
					float x1,y1,x2,y2;
					getMidPoint(subrects[0],x1,y1);
					getMidPoint(subrects[1],x2,y2);				
					float angle=atan2(y2-y1,x2-x1)*(180.0/PI);
					if(x1>x2)
						angle-=180;
					//angle=-angle;
					Point center=Point((x1+x2)/2,(y1+y2)/2);
					rotMat=getRotationMatrix2D(center,angle,1.15);
					cout<<angle<<endl;						
					warpAffine(frame(r),visTrainRoi,rotMat,frame(r).size());
					warpAffine(frameClone(r),trainRoi,rotMat,frameClone(r).size());
					imshow(candidateWindow,visTrainRoi);
					candidateInBuffer=true;
				}

			}
		}		
		imshow(windowName,frame);
		imshow(debugWindow,gray);
		char key=(char) waitKey(1);
		switch(key)
		{
			case 'l':
			case 'L':
				cout<<"Input a label for the training set";
				cin>>directoryToUse;
				break;
			case 'q':
			case 'Q':
				destroyWindow(windowName);
				destroyWindow(debugWindow);
				destroyWindow(candidateWindow);
				return 0;
			case 'e':
			case 'E':
				waitForEyes=!waitForEyes;
				break;
			case 'r':
			case 'R':	//Reject candidate
				candidateInBuffer=false;
				break;
			case 'c':
			case 'C':	//Capture and show a candidate (implied, save the last one)
				getCandidate=true;
				break;
			default:
			break;
		}		
	}
}