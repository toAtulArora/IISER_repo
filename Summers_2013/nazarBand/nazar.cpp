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

// #define PI 3.14159265
using namespace cv;
using namespace std;

int main(int argc, const char *argv[])
{
	string windowName="Camera Stream",debugWindow="Gray-Scale",candidateWindow="Candidate Preview",directoryToUse="Atul";
	Mat frame,frameClone,gray,roi,visRoi;
	ifstream fileTest;
	Size rescaleToSize(200,200);
	
	cout<<"Loading the cascade file for detecting a face"<<endl;
	string cascadeFn = "c:/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
	CascadeClassifier cascade;
	if(!cascade.load(cascadeFn))
		cout<<"Couldn't open the cascade file"<<endl<<cascadeFn;

	cout<<"Loading from trained file for recognizing faces"<<endl;
	Ptr<FaceRecognizer> model=createEigenFaceRecognizer();
	model->load("defaultTrained");	
	// video source = 0 (default camera)
	cout<<"Initializing the default camera"<<endl;
	VideoCapture capture(0);
	if(!capture.isOpened())
		cout<<"Couldn't read from the camera";
	

	cout<<"Creating Windows"<<endl;
	namedWindow(windowName,WINDOW_AUTOSIZE);
	namedWindow(debugWindow,WINDOW_AUTOSIZE);
	namedWindow(candidateWindow,WINDOW_AUTOSIZE);

	for(;;)
	{
		capture>>frame;
		frameClone=frame.clone();
		if(frame.empty())
			cout<<"Can't read from the stream";					
		
		cvtColor(frame,gray,COLOR_BGR2GRAY);
		equalizeHist(gray,gray);
		vector<Rect> rects;
		
		// CascadeClassifier::detectMultiScale(const Mat& image, vector<Rect>& objects, double scaleFactor=1.1, int minNeighbors=3, int flags=0, Size minSize=Size(), Size maxSize=Size())		
		cascade.detectMultiScale(gray,rects,1.3,4,CV_HAAR_SCALE_IMAGE,Size(30,30),Size(300,300));		
		for(Rect r: rects)
		{
			roi=gray(r);			
			Mat roiResize;
			resize(roi, roiResize, rescaleToSize, 0, 0, CV_INTER_AREA);	//for shrinking
			int predictedLabel=model->predict(roiResize);
			cout<<predictedLabel<<endl;

			if(predictedLabel=0)	//Found label 0, Atul
				rectangle(frame,r,Scalar(0,0,255),3);	
			else if(predictedLabel=-1)	//No face found
				rectangle(frame,r,Scalar(255,255,0),3);
			else	//Recognized some other face than label 0 (that's me for now)
				rectangle(frame,r,Scalar(255,0,0));
			
		}		
		imshow(windowName,frame);
		imshow(debugWindow,gray);
		char key=(char) waitKey(1);
		switch(key)
		{
			case 'q':
			case 'Q':
				destroyWindow(windowName);
				destroyWindow(debugWindow);
				destroyWindow(candidateWindow);
				return 0;
			default:
				break;
		}		
	}
}