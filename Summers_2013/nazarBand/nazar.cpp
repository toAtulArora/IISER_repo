#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

#include "boost/filesystem.hpp"
#include "boost/regex.hpp"


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
using namespace boost::filesystem;

void getImages(vector<Mat>& images, vector<int>& labels, vector<string>& sLabels, Size rescaleToSize)
{
	int i=0;
  path current_dir("."); //
	for (directory_iterator itr(current_dir), end; itr != end; ++itr)
  {
    if(is_directory(itr->status()))
    {
      cout<<"\nLooking in "<<itr->path().string()<<"\n";
      cout<<"label:"<<i<<"\nsLabel:"<<itr->path().leaf().string()<<"\n";
      for(directory_iterator iter(itr->path()), end; iter !=end; iter++)        
      {
        if(!is_directory(iter->status()))
        {          
		  Mat img=imread(iter->path().string());
		  if(!img.empty())
		  {
			Mat imgRescale;
			//resize(img, imgRescale, Size(200,200), 0, 0, CV_INTER_CUBIC);
			resize(img, imgRescale, rescaleToSize, 0, 0, CV_INTER_AREA);	//for shrinking
			cout<<"\tImage["<<images.size()<<"] "<<iter->path().leaf().string()<<" added to the said label \n";
			images.push_back(imgRescale);
			labels.push_back(i);
			sLabels.push_back(iter->path().leaf().string());
		  }
		  else
		  {
			cout<<"\tCouldn't read this file as an image: "<<iter->path().leaf().string()<<endl;
		  }
		}
	  }
	  i++;
    }

	}
}

int main(int argc, const char *argv[])
{
	string windowName="Camera Stream",debugWindow="Gray-Scale",candidateWindow="Candidate Preview",directoryToUse="Atul";
	string cascadeFn = "c:/opencv/data/haarcascades/haarcascade_frontalface_alt.xml",projectRoot=".";
	Mat frame,frameClone,gray,roi,roiResize;
	int defaultCamera=0;
	ifstream fileTest;
	Size rescaleToSize(200,200);
	vector<string> Labels;

	cout<<"Initializing common configuration"<<endl;
	FileStorage f("commonConfig",FileStorage::READ);
	if(f.isOpened())
	{		
		f["cascadeFace"]>>cascadeFn;
		f["defaultCamera"]>>defaultCamera;
		f["projectRoot"]>>projectRoot;
		f.release();
	}
	else
	{
		cout<<"Warning: Couldn't read the configuration file";
	}

	cout<<"Loading the cascade file for detecting a face"<<endl;
	
	CascadeClassifier cascade;
	if(!cascade.load(cascadeFn))
		cout<<"Couldn't open the cascade file"<<endl<<cascadeFn;
	cout<<"Success"<<endl;

	cout<<"Loading from trained file for recognizing faces"<<endl;
	Ptr<FaceRecognizer> model=createFisherFaceRecognizer();
	model->load(projectRoot + "/defaultTrained");	
	FileStorage fs(projectRoot + "/defaultTrainedLabels",FileStorage::READ);
	int count;
	fs["count"]>>count;
	cout<<"Total Authorised Personel:"<<count<<endl;
	for(int i=0 ;i<count; i++)
	{
		string s;
		fs[format("l%d",i)] >> s;
		Labels.push_back(s);
		cout<<s<<endl;
	}
	fs.release();	
	cout<<"Loading successful"<<endl;


	//vector<Mat> images;
	//vector<int> labels;
	//vector<string> sLabels;
  	//getImages(images,labels,sLabels,Size(200,200));
	
	//cout<<"Starting Training.."<<endl;
	//Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
	//model->train(images, labels);  
	//cout<<"Done"<<endl;

	// video source = 0 (default camera)
	cout<<"Initializing the default camera:"<<defaultCamera<<endl;
	VideoCapture capture(defaultCamera);
	if(!capture.isOpened())
		cout<<"Couldn't read from the camera";
	

	cout<<"Creating Windows"<<endl;
	namedWindow(windowName,WINDOW_AUTOSIZE);
	namedWindow(debugWindow,WINDOW_AUTOSIZE);
	for(string wName : Labels)
		namedWindow(wName,WINDOW_AUTOSIZE);
	
	//namedWindow(candidateWindow,WINDOW_AUTOSIZE);

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
		cvtColor(frame,gray,COLOR_BGR2GRAY);
		for(Rect r: rects)
		{
			roi=gray(r);
			resize(roi, roiResize, rescaleToSize, 0, 0, CV_INTER_AREA);	//for shrinking
	        int predictedLabel = -1;
	        double confidence = 0.0;
	        model->predict(roiResize, predictedLabel, confidence);

			if(predictedLabel==-1)	//No face found [won't happen unless i use a threshold..]
				rectangle(frame,r,Scalar(255,255,0),1);
			else
			{
				if(confidence<1000)	//Confident
				{
					rectangle(frame,r,Scalar(0,0,255),3);	
					imshow(Labels[predictedLabel],roiResize);
				}
				else	//Not too certain
					rectangle(frame,r,Scalar(255,0,0),1);
		        
				string text=format("%d (%.3f)",predictedLabel,confidence);
				//char text[30];
				text=format("%s (%.3f)",Labels[predictedLabel].c_str(),confidence);				
				putText(frame, text, Point(r.x,r.y-10), FONT_HERSHEY_PLAIN, 1.5, Scalar::all(0), 6, 8);
				putText(frame, text, Point(r.x,r.y-10), FONT_HERSHEY_PLAIN, 1.5, Scalar::all(255), 1, 8);
				
		        //putText(frame, "Hello",textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
				
			}			
		}		
		imshow(windowName,frame);
		imshow(debugWindow,gray);
		//if(!roiResize.empty())
			//imshow(candidateWindow,roiResize);
		char key=(char) waitKey(1);
		switch(key)
		{
			case 'q':
			case 'Q':
				destroyWindow(windowName);
				destroyWindow(debugWindow);
				for(string wName : Labels)
					destroyWindow(wName);
				//destroyWindow(candidateWindow);
				return 0;
			default:
				break;
		}		
	}
}