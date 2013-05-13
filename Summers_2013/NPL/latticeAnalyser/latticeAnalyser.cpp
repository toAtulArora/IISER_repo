// /**
//  * @function findContours_Demo.cpp
//  * @brief Demo code to find contours in an image
//  * @author OpenCV team
//  */

// #include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"
// #include <iostream>
// #include <stdio.h>
// #include <stdlib.h>

// using namespace cv;
// using namespace std;

// Mat src; Mat src_gray;
// int thresh = 100;
// int max_thresh = 255;
// RNG rng(12345);

// /// Function header
// void thresh_callback(int, void* );

// /**
//  * @function main
//  */
// int main( int, char** argv )
// {
//   /// Load source image and convert it to gray
//   src = imread( argv[1], 1 );

//   /// Convert image to gray and blur it
//   cvtColor( src, src_gray, COLOR_BGR2GRAY );
//   blur( src_gray, src_gray, Size(3,3) );

//   /// Create Window
//   const char* source_window = "Source";
//   namedWindow( source_window, WINDOW_AUTOSIZE );
//   imshow( source_window, src );

//   createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
//   thresh_callback( 0, 0 );

//   waitKey(0);
//   return(0);
// }

// /**
//  * @function thresh_callback
//  */
// void thresh_callback(int, void* )
// {
//   Mat canny_output;
//   vector<vector<Point> > contours;
//   vector<Vec4i> hierarchy;

//   /// Detect edges using canny
//   Canny( src_gray, canny_output, thresh, thresh*2, 3 );
//   /// Find contours
//   findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

//   /// Draw contours
//   Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
//   for( size_t i = 0; i< contours.size(); i++ )
//      {
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
//      }

//   /// Show in a window
//   namedWindow( "Contours", WINDOW_AUTOSIZE );
//   imshow( "Contours", drawing );
// }

/**
 * @function generalContours_demo2.cpp
 * @brief Demo code to obtain ellipses and rotated rectangles that contain detected contours
 * @author OpenCV team
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray; Mat srcColorFilter; Mat src_process; Mat srcColorA; Mat srcColorB;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/**
 * @function main
 */
int main( int, char** argv )
{
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );


  // This is a colour filter for improving accuracy
  // 20, 28, 41 [dark]
  // TODO: Allow the user to select the colour
  Scalar dark=Scalar(41,28,20);
  Scalar light=Scalar(145,128,115);
  int lightTol=20;
  int darkTol=20;

  Scalar lowerBound = light-Scalar::all(20);
  Scalar upperBound = light+Scalar::all(20);
  // Now we want a mask for the these ranges
  inRange(src,lowerBound,upperBound, srcColorA);

  srcColorFilter=srcColorA;
  //You may not it here, like so
  // srcColorFilter=Mat(srcColorFilter.rows, srcColorFilter.cols, srcColorFilter.type(), Scalar(255,255,255))-srcColorFilter;
  // srcColorA=Mat(srcColorA.rows, srcColorA.cols, srcColorA.type(), Scalar(255,255,255))-srcColorA;

  // not(srcColorFilter,srcColorFilter);

  /// Convert image to gray and blur it
  cvtColor( src, src_process, COLOR_BGR2GRAY );
  // cvtColor( src, src_gray, COLOR_BGR2GRAY );
  // multiply(src_process,srcColorFilter,src_gray,1);
  src_gray=srcColorFilter.mul(src_process/255);

  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  const char* source_window = "Source";
  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src_gray );


  //Show the filtered image too
  const char* filter_window = "Color Filter";
  namedWindow( filter_window, WINDOW_AUTOSIZE );
  imshow( filter_window, srcColorFilter );


  createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
}

/**
 * @function thresh_callback
 */
void thresh_callback(int, void* )
{
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

  /// Draw contours + rotated rects + ellipses
  Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       // contour
       drawContours( drawing, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       // ellipse
       ellipse( drawing, minEllipse[i], color, 2, 8 );
       // rotated rectangle
       Point2f rect_points[4]; minRect[i].points( rect_points );
       for( int j = 0; j < 4; j++ )
          line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
     }

  /// Show in a window
  namedWindow( "Contours", WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}
