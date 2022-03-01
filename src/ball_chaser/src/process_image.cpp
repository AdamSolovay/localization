#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;

void drive_bot(float lin_x, float ang_z)
{
  //ROS_INFO_STREAM("Drive robot to white ball");

	ball_chaser::DriveToTarget drive;
	drive.request.linear_x = lin_x;
	drive.request.angular_z = ang_z;

	if (!client.call(drive))
		ROS_ERROR("Failed to call drive");
}

void process_image_callback(const sensor_msgs::Image img)
{
	int white_pixel = 255;
	bool ballFound = false;
	int pixelLocation = 0;

	for (int i = 0; i < img.height*img.step; i+= 3){
		if (img.data[i] == white_pixel && img.data[i+1] == white_pixel && img.data[i+2] == white_pixel){ 
			pixelLocation = i % img.step;
			ballFound = true;
			break;
		} else {
			ballFound = false;
		}
	}

	if (!ballFound){
		drive_bot(0.0,0.0);
	} else {
		if (pixelLocation <= img.step/3){
			drive_bot(0.0,0.1);
		} else if (pixelLocation > img.step/3 && pixelLocation < img.step*2/3){
			drive_bot(0.1,0.0);
		} else if (pixelLocation >= img.step*2/3){
			drive_bot(0.0,-0.1);
		}
	}

}

int main(int argc, char** argv)
{

	ROS_INFO("INITIALIZE PROCESS IMAGE NODE");
	ros::init(argc, argv, "process_image");
	ros::NodeHandle n;

	client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

	ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);


	ros::spin();
	return 0;

}
