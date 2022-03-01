#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

ros::Publisher motor_command_publisher;

// TODO: Create a Handle Drive Request


bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{

	geometry_msgs::Twist motor_command;

	motor_command.linear.x = req.linear_x;
	motor_command.angular.z = req.angular_z;

	motor_command_publisher.publish(motor_command);

	res.msg_feedback = "linear x: " + std::to_string(req.linear_x) + " , angular z: " + std::to_string(req.angular_z);
	
	ROS_INFO_STREAM(res.msg_feedback);

	return true;
}


int main(int argc, char** argv)
{

	ros::init(argc, argv, "drive_bot");
	ros::NodeHandle n;

	motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

	//TODO: Define a drive /ball_chaser/command_robot service

	//TODO: Delete loop, move code inside callback function

	ros::ServiceServer service = n.advertiseService("ball_chaser/command_robot", handle_drive_request);


	//TODO: Handle ROS comm events
	ros::spin();

	return 0;

}
