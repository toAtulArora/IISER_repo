/*
  filename: latticeAnalyser.cpp
  description: This is the main application which analyses the lattice and 
                  1. controls `temperature'
                  2. records dipole position (thus angular velocity) as a function of time
  baby steps (TM):
    1. Proof of concept stage
        a. Find a suitable algorithm
        b. Make the required modifications
            i. Add a colour filter
              Implement two colours [done]
              Add two sliders for adjusting tolerance [done, but need to refresh something!]
              Add GUI for selecting the colours [done, but not a gui so to speak]
              save settings [not doing it]
    2. Look at it grow!
        a. Enable screen cropping [done]
        b. Write an algorithm for ellipse to dipole conversion [in process]
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
// #include <array> 

using namespace cv;
using namespace std;

Mat srcPreCrop; Mat src; Mat src_gray; Mat srcColorFilter; Mat src_process; Mat srcColorA; Mat srcColorB;
//for the cropping
int cropped = 0;
Point origin;
Rect selection;
bool selectRegion;

// Mat cimg;
Mat_<Vec3b> srcTemp = src;
int thresh = 100;
int max_thresh = 255;
int canny=100;
int centre=30;
int minMinorAxis=1, maxMajorAxis=30;
int mode=0;
float theta=3.14159;


//0 is screen select
//1 is colour select

RNG rng(12345);

class dipole
{
public:
  float angle; //angle
  int x,y;  //centre
  int e1,e2; //index number of ellipse
  static int count[2]; //double buffer
  static int current;

};

int dipole::count[2] = {0};
int dipole::current=0;

//Not using a dynamic array, doesn't matter for now
//TODO: Use a dynamic aray
// array<dipole,500> dipoles;
// array<dipole,500> lastDipoles;
dipole dipoles[2][5000];
// Colour read
// Point origin;

// This is a colour filter for improving accuracy
  // 20, 28, 41 [dark]
  // TODO: Allow the user to select the colour
  Scalar colorB=Scalar(245,245,10);
  Scalar colorA=Scalar(10,245,245);
  int colorATol=30;
  int colorBTol=30;
//
  const char* source_window = "Source";
  const char* filter_window = "Color Filter";
  const char* settings_window="Settings";

static void onMouse( int event, int x, int y, int, void* )
{
  if(mode==0)
  {
    if( selectRegion )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        selection &= Rect(0, 0, src.cols, src.rows);
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        cropped=0;
        origin = Point(x,y);
        selection = Rect(x,y,0,0);
        selectRegion = true;
        break;
    case CV_EVENT_LBUTTONUP:
        cropped=0;
        selectRegion = false;
        if( selection.width > 0 && selection.height > 0 )
            cropped = -1;
        break;
    }    
  }
  else if(mode==1)
  {
    switch( event )
    {        
    case CV_EVENT_LBUTTONUP:
        cout<<x<<","<<y<<endl;
        colorA=Scalar(src.at<Vec3b>(x,y)[0],src.at<Vec3b>(x,y)[1],src.at<Vec3b>(x,y)[2]);        
        cout<<"Color A's been changed to "<<endl<<colorA.val[0]<<endl<<colorA.val[1]<<endl<<colorA.val[2]<<endl;
        // displayOverlay("Source","haha",2000);
        // cout<<colorA.val;
        break;
    case CV_EVENT_RBUTTONUP:
        cout<<x<<","<<y<<endl;
        colorB=Scalar(src.at<Vec3b>(x,y)[0],src.at<Vec3b>(x,y)[1],src.at<Vec3b>(x,y)[2]);
        cout<<"Color B's been changed to "<<endl<<colorB.val[0]<<endl<<colorB.val[1]<<endl<<colorB.val[2]<<endl;
        break;
    }
  }
}



int process(VideoCapture& capture)
{
  
  for(;;)
  {    

    {  //IMAGE CAPTURE and CROP  
      capture>>srcPreCrop;    
      if(srcPreCrop.empty())
      {
        cout<<"Didn't get an image";
        break;
      }
      if(!cropped==0)
      {
        src=srcPreCrop(selection);
      }
      else
        src=srcPreCrop;
      imshow( source_window, srcPreCrop );
    }

    ////////////////////////
    {//COLOR FILTER
      //Input src, output src_gray
      Scalar lowerBound;
      Scalar upperBound;

      lowerBound = colorA-Scalar::all(colorATol);
      upperBound = colorA+Scalar::all(colorATol);
      // Now we want a mask for the these ranges
      inRange(src,lowerBound,upperBound, srcColorA);

      lowerBound = colorB-Scalar::all(colorBTol);
      upperBound = colorB+Scalar::all(colorBTol);  
      // We do it for both the colours 
      inRange(src,lowerBound,upperBound, srcColorB);

      // Now we create a combined filter for them
      addWeighted(srcColorA, 1, srcColorB, 1, 0, srcColorFilter);
      

      /// Convert image to gray
      cvtColor( src, src_process, COLOR_BGR2GRAY );

      /// Now keep only the required areas in the image  
      // // // multiply(src_process,srcColorFilter,src_gray,1);
      src_gray=srcColorFilter.mul(src_process/255);
      // // // src_gray=srcColorFilter;

      // NOw blur it
      blur( src_gray, src_gray, Size(3,3) );

      imshow( filter_window, src_gray);
    }

    ////////////////////////////

    // BLANK PROCESSING
    // medianBlur( src, src, 5 );
    // cvtColor( src, src_gray, COLOR_BGR2GRAY );

    // // // blur( src_gray, src_gray, Size(3,3) );

    /////////////////////////////    
    // This is contour Detection
    /////////////////////////////////
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using Threshold
    threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
    /// Find contours
    findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Find the rotated rectangles and ellipses for each contour
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );

    for( size_t i = 0; i < contours.size(); i++ )
       { minRect[i] = minAreaRect( Mat(contours[i]) );
         if( contours[i].size() > 5 )
           { minEllipse[i] = fitEllipse( Mat(contours[i]) ); }
       }

    for( size_t i = 0; i< contours.size(); i++ )
       {
        if(!(
          (minEllipse[i].size.height>minMinorAxis && minEllipse[i].size.width>minMinorAxis) 
          &&
          (minEllipse[i].size.height<maxMajorAxis && minEllipse[i].size.width<maxMajorAxis)
          ))
        {
          minEllipse[i]=RotatedRect(Point2f(0,0),Point2f(0,0),0);
        }
       }

    

    ///////////////////////////
    //  Dipole Detection Algorithm
    ///////////////////////////    
    bool detected[5000]={0};

    int k = !(dipoles[0][0].current);
    dipoles[0][0].current=k;
    dipoles[0][0].count[k]=0;
    
    // dipolesA[0].lastcount=0;
    for (int i=0; i<contours.size();i++)
    {
      if(detected[i]==false)
      {
        for (int j=0; j<contours.size();j++)
        {
          if((i!=j) && detected[j]==false)  //This is so that you don't test with yourself and with others that got paired
          {
              // if(abs(minEllipse[i].angle-minEllipse[j].angle)< 15)  //if the orientation matches (less than 5 degrees), then 
              {
                //Find the distance between minEllipses

                Point differenceVector=Point(minEllipse[i].center.x - minEllipse[j].center.x, minEllipse[i].center.y - minEllipse[j].center.y);  //find the difference vector
                float distanceSquared=differenceVector.x*differenceVector.x + differenceVector.y*differenceVector.y; //find the distance squared

                //Find the major axis length
                float majorAxis=MAX( MAX(minEllipse[i].size.width, minEllipse[i].size.height) , MAX(minEllipse[j].size.width, minEllipse[j].size.height)); //find the max dimension
                
                //The ratio is the ratio between the distance between the ellipse and the small circle and the length of the major axis
                float errorPlusOne = distanceSquared / ((11.348/30)*(11.348/30)*majorAxis*majorAxis) ; //now to compare, just divide and see if it's close enough to one

                if (errorPlusOne>0.5 && errorPlusOne<2)  //if the error is small enough, then its a match
                {
                    //This is to ensure these don't get paired
                    detected[i]=true;
                    detected[j]=true;

                    //this is collection of the final result
                    int c=dipoles[k][0].count[k]++; //dont get confused, count is static, so even dipoles[0][0] would've worked, ro for that matter, any valid index
                    //Note the ++ is after because the count is always one greater than the index of the last element!
                    dipoles[k][c].x=(minEllipse[i].center.x + minEllipse[j].center.x)/2.0;
                    dipoles[k][c].y=(minEllipse[i].center.y + minEllipse[j].center.y)/2.0;
                    // dipoles[k][c].angle=(minEllipse[i].angle + minEllipse[j].angle)/2.0;
                    // dipoles[k][c].angle=(minEllipse[i].angle);

                    // We're using two shapes, one ellipse and one circle.                    
                    RotatedRect largerEllipse =  ( MAX(minEllipse[i].size.width, minEllipse[i].size.height) > MAX(minEllipse[j].size.width, minEllipse[j].size.height)  )?minEllipse[i]:minEllipse[j];
                    RotatedRect smallerEllipse =  ( MAX(minEllipse[i].size.width, minEllipse[i].size.height) <= MAX(minEllipse[j].size.width, minEllipse[j].size.height)  )?minEllipse[i]:minEllipse[j];
                    dipoles[k][c].angle=(largerEllipse.angle);

                    //Now we use the circle to remove the mod 180 problem and get the complete 360 degree position
                    if((smallerEllipse.center.y -largerEllipse.center.y) < 0)
                      dipoles[k][c].angle+=180;

                    dipoles[k][c].e1=i; //don't know why this is required
                    dipoles[k][c].e2=j;


                }
                // magnitude(differenceVector.x,differenceVector.y,distance);

                // point positionVector ((minEllipse[i].x + minEllipse[j].x)/2.0,(minEllipse[i].y + minEllipse[j].y)/2.0);                

              }
          }
        } 
      }
    }

    //////////////DRAWING THE CONTOUR AND DIPOLE
    /// Draw contours + rotated rects + ellipses
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
       {
         // Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        Scalar color = Scalar(0,0,255 );
         // contour
         drawContours( drawing, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );

         // ellipse
         
         ellipse( drawing, minEllipse[i], color, 2, 8 );

         // rotated rectangle
         // Point2f rect_points[4]; minRect[i].points( rect_points );
         // for( int j = 0; j < 4; j++ )
         //    line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
        // }


        // int xx=dipoles[k][i].x;
        // int yy=dipoles[k][i].y;
        // int theta=dipoles[k][i].angle;

        // line(drawing, Point2f(xx,yy),Point2f(xx + 5*cos(theta), yy + 5*sin(theta)), Scalar(0,0,255),1,8);






       }

    for( int i=0;i<dipoles[0][0].count[k];i++)
    {

      // Use "y" to show that the baseLine is about
      char text[30];
      // dipoles[0][0].count[0]=1;
      // sprintf(text,"%f",dipoles[0][dipoles[0][0].count[k]-1].angle);
      sprintf(text,"%1.1f",dipoles[k][i].angle);
      int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
      double fontScale = 0.5;
      int thickness = 1;

      int baseline=0;
      Size textSize = getTextSize(text, fontFace,
                                  fontScale, thickness, &baseline);
      baseline += thickness;

      // center the text
      Point textOrg((drawing.cols - textSize.width)/2,
                    (drawing.rows + textSize.height)/2);
        // // draw the box
        // rectangle(drawing, textOrg + Point(0, baseline),
        //           textOrg + Point(textSize.width, -textSize.height),
        //           Scalar(0,0,255));
        // // ... and the baseline first
        // line(drawing, textOrg + Point(0, thickness),
        //      textOrg + Point(textSize.width, thickness),
        //      Scalar(0, 0, 255));

        // then put the text itself
        // putText(drawing, text, textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
      putText(drawing, text, Point(dipoles[k][i].x,dipoles[k][i].y), fontFace, fontScale, Scalar::all(255), thickness, 8);

      int xx=dipoles[k][i].x;
      int yy=dipoles[k][i].y;
      float theta = (3.1415926535/180) * dipoles[k][i].angle;

      line(drawing, Point2f(xx - 5*cos(theta), yy - 5*sin(theta)),Point2f(xx + 5*cos(theta), yy + 5*sin(theta)), Scalar(0,255,255),5,8);
    }
    imshow( "Contours", drawing );
  


    ///////////////////////////    
    //  THIS IS HOUGH
    ///////////////////////////
    // // cvtColor(img, cimg, CV_GRAY2BGR);
    // // cimg=src_gray;
    // // Mat cimg();
    // Mat cimg(src.rows,src.cols, CV_8UC3, Scalar(255,255,255));

    // vector<Vec3f> circles;
    // HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, 10,
    //              canny, centre, minMinorAxis, maxMajorAxis // change the last two parameters
    //                             // (min_radius & max_radius) to detect larger circles
    //              );

    // // src_gray:s Input image (grayscale)
    // // circles: A vector that stores sets of 3 values: x_{c}, y_{c}, r for each detected circle.
    // // CV_HOUGH_GRADIENT: Define the detection method. Currently this is the only one available in OpenCV
    // // dp = 1: The inverse ratio of resolution
    // // min_dist = src_gray.rows/8: Minimum distance between detected centers
    // // param_1 = 200: Upper threshold for the internal Canny edge detector
    // // param_2 = 100*: Threshold for center detection.
    // // min_radius = 0: Minimum radio to be detected. If unknown, put zero as default.
    // // max_radius = 0: Maximum radius to be detected. If unknown, put zero as default

    // for( size_t i = 0; i < circles.size(); i++ )
    // {
    //     Vec3i c = circles[i];
    //     // Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );        
    //     Scalar color = Scalar( 255,255,0 );        
    //     circle( cimg, Point(c[0], c[1]), c[2], color, 3, CV_AA);
    //     circle( cimg, Point(c[0], c[1]), 2, color, 3, CV_AA);
    // }

    // imshow("Hough", cimg);


    //////////////////////
    // CLI
    //////////////////////
    char key = (char) waitKey(5); //delay N millis, usually long enough to display and capture input
    switch (key)
    {
        case 'c':
          mode=1;
          cout<<"Mouse will capture color now. Right click for one, left for the other";
          break;
        case 's':
          mode=0;
          cout<<"Screen crop mode selected. Mouse will capture start point at left click and the other point at right click";
          break;
        case 'p':
          cout<<"Processing..";
          for( int i=0;i<dipoles[0][0].count[0];i++)
            {
              cout<<"Hello";
            }
          break;                    
        case 'q':
        case 'Q':
        case 27: //escape key
            return 0;
        // case ' ': //Save an image
        //     sprintf(filename, "filename%.3d.jpg", n++);
        //     imwrite(filename, frame);
        //     cout << "Saved " << filename << endl;
        //     break;
        default:
            break;
    }
  }
  return 0;


}

/**
 * @function main
 */
int main( int ac, char** argv )
{

  ////Voodoo intializations
  // dipoles[0][0].current=0;
  // dipoles[0][0].count[0]=0;
  // dipoles[0][0].count[1]=0;


  /// Create Window

  namedWindow( source_window, WINDOW_AUTOSIZE );
  setMouseCallback( "Source", onMouse, 0 );
  // createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback);
  //CAN BE ENABLED, but causes problems, the following lines, to the color detection
  // createTrackbar( " Threshold:", "Source", &thresh, max_thresh, 0);


  //Show the filtered image too

  namedWindow( filter_window, WINDOW_AUTOSIZE );

  //Show the settings window

  namedWindow(settings_window,WINDOW_AUTOSIZE  | CV_GUI_NORMAL);
  createTrackbar( "ColorA Tolerance", settings_window, &colorATol, 256, 0 );
  createTrackbar( "ColorB Tolerance", settings_window, &colorBTol, 256, 0 );
  createTrackbar( "Min Radius (Hough)", settings_window, &minMinorAxis, 100, 0 );
  createTrackbar( "Max Radius (Hough)", settings_window, &maxMajorAxis, 200, 0 );  
  createTrackbar( "Canny (Hough)", settings_window, &canny, 200, 0 );  
  createTrackbar( "Centre (Hough)", settings_window, &centre, 200, 0 );    
  // createTrackbar( "Theta", settings_window, &thetaD, 3.141591, 0 );    

  /// Show in a window
  namedWindow( "Contours", WINDOW_AUTOSIZE );
  namedWindow( "Hough", WINDOW_AUTOSIZE );


  /// Load source image
  // src = imread( argv[1], 1 );
  std::string arg = argv[1];
  VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file
  if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
      capture.open(atoi(arg.c_str()));
  if (!capture.isOpened())
  {
      cerr << "Failed to open a video device or video file!\n" << endl;      
      return 1;
  }
  
  process(capture); 

  return 0;
}
