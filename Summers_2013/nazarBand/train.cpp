#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

#include "boost/filesystem.hpp"
#include "boost/regex.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

//#include <cctype>

using namespace std;
using namespace cv;
using namespace boost::filesystem;

void getImages(vector<Mat>& images, vector<int>& labels, vector<string>& sLabels, Size rescaleToSize,string projectRoot)
{
	int i=0;
  path current_dir(projectRoot); //
	for (directory_iterator itr(current_dir), end; itr != end; ++itr)
  {
    if(is_directory(itr->status()))
    {
      cout<<"\nLooking in "<<itr->path().string()<<"\n";
      cout<<"label:"<<i<<"\nsLabel:"<<itr->path().leaf().string()<<"\n";
      sLabels.push_back(itr->path().leaf().string());
      for(directory_iterator iter(itr->path()), end; iter !=end; iter++)        
      {
        if(!is_directory(iter->status()))
        {          
		  Mat img=imread(iter->path().string(),CV_LOAD_IMAGE_GRAYSCALE);
		  if(!img.empty())
		  {
			Mat imgRescale;
			//resize(img, imgRescale, Size(200,200), 0, 0, CV_INTER_CUBIC);
			resize(img, imgRescale, rescaleToSize, 0, 0, CV_INTER_AREA);	//for shrinking
			cout<<"\tImage["<<images.size()<<"] "<<iter->path().leaf().string()<<" added to the said label \n";
			images.push_back(imgRescale);
			labels.push_back(i);
			// sLabels.push_back(itr->path().leaf().string());
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
	// if(images.size()>0)
	// 	return 0;
	// else
	// 	return 1;
}

int main()
{
	string projectRoot=".";
	vector<Mat> images;
	vector<int> labels;
	vector<string> Labels;
	cout<<"Reading common configuration file"<<endl;
	FileStorage f("commonConfig",FileStorage::READ);
	if(f.isOpened())
	{		
		f["projectRoot"]>>projectRoot;
		f.release();
	}
	else
	{
		cout<<"Warning: Couldn't read the configuration file";
	}

  getImages(images,labels,Labels,Size(200,200),projectRoot);
	
  if(images.size() <=1 )
  {
    cout<<"Too few images to train\n";
	return 1;
  }
  cout<<"Starting Training.."<<endl;
  Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
  model->train(images, labels);  
  cout<<"Done"<<endl;

  cout<<"Saving to file.."<<endl;
  model->save(projectRoot + "/defaultTrained");
  FileStorage fs(projectRoot + "/defaultTrainedLabels",FileStorage::WRITE);
  fs<<"count"<< (int)(Labels.size()) ;
  for(string s : Labels)
  {
    static int i=0;	
    fs << format("l%d",i++) << s;
  }
  fs.release();
  cout<<"Done"<<endl;

  cout<<endl<<"Press enter to quit"<<endl;
  char a=0;
  while(a!='\n')
  {
	  a=cin.get();
  }  

	return 0;
}