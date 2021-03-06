#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/image_encodings.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include <iostream>
#include <ctype.h>
#include "opencv2/core/core.hpp"
#include <opencv2/video/background_segm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "motion_detector/Mode.h"
#include "std_srvs/Empty.h"


std::string mode = "RAW";
//cv::Point prev1;
//cv::Point prev2;

static const std::string MOG2 = "MOG2";
static const std::string FBCK = "FARNEBACK";
static const std::string CONTOUR = "CONTOUR";
static const std::string PASSTHROUGH = "PASSTHROUGH";
<<<<<<< HEAD
=======

//RNG rng(12345);
>>>>>>> 26049c9220dcd8da9552dfd6e306c1654097b76f


using namespace cv;
using namespace std;

class ImageConverter {

  ros::NodeHandle node;
  image_transport::ImageTransport img_transport;
  image_transport::Subscriber image_sub;
  image_transport::Publisher image_pub;

  
  Mat cv_image_copy;
  Mat frame; 
  Mat fgMaskMOG2; 
  Mat prevFrame;
  BackgroundSubtractorMOG2* bg_model;
<<<<<<< HEAD
  int erosion_size = 10;
  int dilation_size = 10;
=======
  int erosion_size = 11;
  int dilation_size = 9; 
>>>>>>> 26049c9220dcd8da9552dfd6e306c1654097b76f

  
  Mat erodelement = getStructuringElement(cv::MORPH_ELLIPSE,
              cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
              cv::Point(erosion_size, erosion_size) );


  Mat dilatelement = getStructuringElement(cv::MORPH_ELLIPSE,
              cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
              cv::Point(dilation_size, dilation_size) );

public:
  
  ImageConverter() : img_transport(node) {
	
	bg_model = new BackgroundSubtractorMOG2(100,16,true);
   	// Subscribe to input video feed and publish output video feed
    	image_sub = img_transport.subscribe("/usb_cam/image_raw", 1, 
					    &ImageConverter::imageCb, this);
   	image_pub = img_transport.advertise("/image_converter/output_video", 
					    1);
<<<<<<< HEAD
    	cv::namedWindow(MOG2);
	cv::namedWindow(FBCK);
	cv::namedWindow(CONTOUR);
	cv::namedWindow(PASSTHROUGH);
=======
>>>>>>> 26049c9220dcd8da9552dfd6e306c1654097b76f
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg) {
    	cv_bridge::CvImagePtr cv_ptr;
	try {
      		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    	} catch (cv_bridge::Exception& e) {
      		ROS_ERROR("cv_bridge exception: %s", e.what());
  
    		return;
    	}
	
	//MOG2 BACKGROUND SUBTRACTION//
//	ImageConverter::backgroundSubtraction(cv_ptr->image);

	//Farneback Optical Flow//
//	ImageConverter::imageFarneback(cv_ptr->image);

//	cv_ptr2=cv_ptr->image;

<<<<<<< HEAD
	//Passthrough Video//
	ImageConverter::imagePassthrough(cv_ptr->image);

	//image_pub.publish(cv_ptr->toImageMsg());
  }

  static void imagePassthrough(Mat cv_image) {
	cv::imshow(PASSTHROUGH, cv_image);
  }
	
=======
	image_pub.publish(cv_ptr->toImageMsg());


	if(mode == "MOG2") {
		destroyWindow(FBCK);
		destroyWindow(PASSTHROUGH);
//		cv::namedWindow(MOG2);
		cv::namedWindow(CONTOUR);
		ImageConverter::backgroundSubtraction(cv_ptr->image);
	}
	//Farneback Optical Flow//
	if(mode=="FARNEBACK") {
//		destroyWindow(MOG2);
		destroyWindow(CONTOUR);
		destroyWindow(PASSTHROUGH);
		cv::namedWindow(FBCK);
		ImageConverter::imageFarneback(cv_ptr->image);
	}
        if(mode=="RAW") {
  //              destroyWindow(MOG2);
                destroyWindow(CONTOUR);
		destroyWindow(FBCK);
                cv::namedWindow(PASSTHROUGH);
		ImageConverter::imagePassthrough(cv_ptr->image);
        }



//	image_pub.publish(cv_ptr->toImageMsg());
  }


static void imagePassthrough(Mat cv_image) {
	cv::imshow(PASSTHROUGH, cv_image);
	cv::waitKey(3);
  }


>>>>>>> 26049c9220dcd8da9552dfd6e306c1654097b76f
  static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, 
			     int step, double, const Scalar& color) {

	for(int y = 0; y < cflowmap.rows; y += step)
        	for(int x = 0; x < cflowmap.cols; x += step) {
			const Point2f& fxy = flow.at<Point2f>(y, x);
			line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), 
			     cvRound(y+fxy.y)), color);
            		circle(cflowmap, Point(x,y), 2, color, -1);
        	}
  }

<<<<<<< HEAD
  void drawBoundingBox(Mat cv_image1,Mat cv_image2) {
=======
/***********************************************************************/
>>>>>>> 26049c9220dcd8da9552dfd6e306c1654097b76f


void drawBoundingBox(Mat cv_image1,Mat cv_image2) {

        Mat maskImg;
        Mat thresh_output;
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        int thresh = 126;
        maskImg=cv_image1;
        //cv::cvtColor(cv_image, maskImg, CV_BGR2GRAY);
        cv::blur(maskImg, maskImg, Size(3,3));
        //dilate(maskImg,maskImg,element);
        threshold(maskImg, thresh_output, thresh, 255, THRESH_BINARY);
        findContours(thresh_output, contours, hierarchy, CV_RETR_TREE, 
                     CV_CHAIN_APPROX_SIMPLE, Point(0,0));


        vector<vector<Point>> contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );

<<<<<<< HEAD
	Mat drawing = Mat::zeros( thresh_output.size(), CV_8UC3 );
	cv::Point prev1;
	cv::Point prev2;
  	for( int i = contours.size()-1; i> contours.size()-2; i-- )
     	{
		if(contourArea(contours[i])>600.0){
			rectangle(drawing, boundRect[i].tl(), 
				  boundRect[i].br(),CV_RGB(0,255,0),2,8,0);
			prev1=boundRect[i].tl();
			prev2=boundRect[i].br();
			if(boundRect[i-1].tl().x<prev1.x && 
				boundRect[i-1].tl().y<prev1.y && 
				boundRect[i-1].br().x>prev2.x && 
				boundRect[i-1].br().y>prev2.y) {
                		rectangle(drawing, boundRect[i].tl(), 
				  	boundRect[i].br(), CV_RGB(0,255,0),2,8,0);
			}
		}
     	}
	cv::imshow(CONTOUR, drawing);
  }
 
  void backgroundSubtraction(Mat cv_image) {

	bg_model->operator()(cv_image, fgMaskMOG2, 0.15);
=======
        for( int i = 0; i < contours.size(); i++ ) { 
                approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
                boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        }



        Mat drawing = cv_image2;//Mat::zeros( thresh_output.size(), CV_8UC3 );
        for( int i = contours.size()-1; i> contours.size()-5; i-- )
        {
                if(contourArea(contours[i])>800.0){
                rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), CV_RGB(0,255,0), 2, 8, 0 );
                }
        }
        cv::imshow(CONTOUR, drawing);//thresh_output);
  }





/********************************************************/ 
  void backgroundSubtraction(Mat cv_image) {

	bg_model->operator()(cv_image, fgMaskMOG2, 0.2);
>>>>>>> 26049c9220dcd8da9552dfd6e306c1654097b76f
	
//	erode(fgMaskMOG2,fgMaskMOG2,erodelement);
	dilate(fgMaskMOG2,fgMaskMOG2,dilatelement);
	erode(fgMaskMOG2,fgMaskMOG2,erodelement);

//	cv::imshow(MOG2, fgMaskMOG2);
     	
	ImageConverter::drawBoundingBox(fgMaskMOG2,cv_image);
	cv::waitKey(3);
  }
	
  void imageFarneback(Mat cv_image) {

	Mat gray;
	Mat prevgray;
	Mat flow;
	Mat cflow;

	cv::cvtColor(cv_image, gray, CV_BGR2GRAY);
	 
	if (!prevFrame.empty()) {
		cv::cvtColor(prevFrame, prevgray, CV_BGR2GRAY);
		cv::calcOpticalFlowFarneback(prevgray, gray, flow, 
					     0.5, 3, 15, 3, 5, 1.2, 0);
		cv::cvtColor(prevgray, cflow, CV_GRAY2BGR);
		ImageConverter::drawOptFlowMap(flow, cflow, 16, 1.5, 
					       CV_RGB(0, 255, 0));
		cv::imshow(FBCK, cflow);
	} 
	
	prevFrame = cv_image;
	cv::waitKey(3);
  }


  ~ImageConverter() {
    cv::destroyWindow(MOG2);
    cv::destroyWindow(FBCK);
    cv::destroyWindow(CONTOUR);
    cv::destroyWindow(PASSTHROUGH);
  }
};




bool exitCallback(std_srvs::Empty::Request &req,
                  std_srvs::Empty::Response &res)
{
        ros::shutdown();

        return true;
}

/******************************
 *
 * SETUP MODE CALLBACK HERE.
 *
 ******************************/
bool modeCallback(motion_detector::Mode::Request  &req,
                  motion_detector::Mode::Response &res)
{

        mode = req.mode;
        res.success = bool(true);
//	ImageConverter ic;
        return true;
}





int main(int argc, char** argv) {
  ros::init(argc, argv, "image_converter");


  ros::NodeHandle node;

  //ros::ServiceServer mathServer = node.advertiseService("math_two_nums", mathCallback);

  ros::ServiceServer exitServer = node.advertiseService("exit", exitCallback);

        /******************************
         *
         * SETUP THE MODE SERVER HERE.
         *
         ******************************/
  ros::ServiceServer modeServer = node.advertiseService("mode", modeCallback);


  ImageConverter ic;
  ros::spin();
  return 0;
}
