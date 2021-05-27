#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include <scavenger_hunt_msgs/Proof.h>
#include <sstream>
#include <scavenger_hunt_msgs/GetHunt.h>
#include <scavenger_hunt_msgs/Hunt.h>
#include <scavenger_hunt_msgs/Task.h>
#include <scavenger_hunt_msgs/SendProof.h>
#include <sensor_msgs/Image.h>
#include <scavenger_hunt/huntBU.h>

#include <iostream>
using namespace std;


scavenger_hunt_msgs::Proof proof;
scavenger_hunt_msgs::SendProof send_proof;
sensor_msgs::Image image;
bool sent = false;

ros::ServiceClient client_get_hunt, client_get_proof_status, client_send_proof;
ros::ServiceClient task_execution_client;
scavenger_hunt_msgs::GetHunt get_hunt;
scavenger_hunt_msgs::Hunt hunt;

void save_image(const sensor_msgs::Image::ConstPtr& msg) {
  if (sent)
    return;

ROS_INFO("Shall I take photo? (Hit 1, if you want the photo to be taken)");
int x;
cin >> x;
if (x==1){
image = *msg;	
}
  sent = true;
  
  proof.image = image;
  scavenger_hunt_msgs::Task task = get_hunt.response.hunt.tasks[0];
  task = hunt.tasks[0];
 
    std_msgs::String msg2;
    std::stringstream ss;
    ss << "Task name is  " << task.name;
    msg2.data = ss.str();

    ROS_INFO("%s", msg2.data.c_str());

  send_proof.request.task = task;
  send_proof.request.proof = proof;
 

  client_send_proof.call(send_proof);
  unsigned long proof_id = send_proof.response.id;

ROS_INFO("sent the proof!");
ROS_INFO("%i",proof_id); 
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "bu_node");

  ros::NodeHandle nh;


   client_get_hunt = nh.serviceClient<scavenger_hunt_msgs::GetHunt>(
    "/scavenger_hunt/get_hunt"
   );


	get_hunt.request.hunt_name = "visit_offices"; // Name of the hunt to download
	client_get_hunt.call(get_hunt);
	hunt = get_hunt.response.hunt;


	client_send_proof = nh.serviceClient<scavenger_hunt_msgs::SendProof>(
	  "/scavenger_hunt/send_proof"
	); 

	task_execution_client = nh.serviceClient<scavenger_hunt::huntBU>("visit_service");  

	ros::Rate loop_rate(1);

	int time_task_start;  

	time_task_start = ros::Time::now().toSec();
	ros::Duration(1.0).sleep(); 

	ROS_INFO("SHall I start executing visit location task?(Hit 1 to say yes");
	int x;
	cin >> x; // Get user input from the keyboard	 

	scavenger_hunt::huntBU srv;
	if (x==1){
		

		task_execution_client.call(srv);
	}

	ROS_INFO("Ready!");
    
    ROS_INFO("Once the task is finished, hit '1' to verify task execution");

	int y;
	cin >> y; // Get user input from the keyboard

	if (y == 1){
	proof.task_duration = ros::Time::now().toSec() - time_task_start;
}

ros::Subscriber sub0 = nh.subscribe("/camera/rgb/image_raw",1,save_image);



  ros::spin();


	

  return 0;
}