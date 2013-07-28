#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

#include <iostream>
#include <iterator>
#include <cctype>
#include <vector>
// #include <fstream>
// #include <sstream>

using namespace cv;
using namespace std;

int main(int argc, const char *argv[])
{
	string windowName="Camera Stream",debugWindow="Gray Scale";
	namedWindow(windowName,WINDOW_AUTOSIZE);
	namedWindow(debugWindow,WINDOW_AUTOSIZE);
	Mat frame,gray,roi,visRoi;
	// bool getCandidate;
	
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
			nestedCascade.detectMultiScale(roi,subrects,1.3,4,CV_HAAR_SCALE_IMAGE,Size(30,30),Size(300,300));
			rectangle(frame,r,Scalar(255,0,0));
			visRoi=frame(r);
			for(Rect rr: subrects)
				rectangle(visRoi,(rr),Scalar(0,255,0));
		}		
	// vector<int> compression_params;
    // compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    // compression_params.push_back(9);
	// imwrite("alpha.png",mat,compression_params);
		imshow(windowName,frame);
		imshow(debugWindow,gray);
		char key=(char) waitKey(1);
		switch(key)
		{
			case 'q':
			case 'Q':
				return 0;
			// case 'c':
			// case 'C':
			// 	getCandidate=true;
			// 	break;
			default:
			break;
		}		
	}
}