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
#include <string>
// usefull : http://library.isr.ist.utl.pt/docs/roswiki/ROS(2f)Tutorials(2f)WritingServiceClient(28)c(2b2b29).html
//useful: 

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
scavenger_hunt_msgs::Task temp_task;

void save_image(const sensor_msgs::Image::ConstPtr& msg) {
  if (sent)
    return;

ROS_INFO("Shall I take photo? (y)");
string x;
cin >> x;
if (x=="y"){
image = *msg;	
}
  sent = true;
  
  proof.image = image;
  //scavenger_hunt_msgs::Task temp_task = get_hunt.response.hunt.tasks[0];
  //temp_task = hunt.tasks[0];
 
    std_msgs::String msg2;
    std::stringstream ss;
    ss << "Task name is  " << temp_task.name;
    msg2.data = ss.str();

    ROS_INFO("%s", msg2.data.c_str());

  send_proof.request.task = temp_task;
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


	get_hunt.request.hunt_name = "visit_offices"; // Name of the hunt to download, potentially can be received as an input arg
	client_get_hunt.call(get_hunt);
	hunt = get_hunt.response.hunt;


	client_send_proof = nh.serviceClient<scavenger_hunt_msgs::SendProof>(
	  "/scavenger_hunt/send_proof"
	); 

	task_execution_client = nh.serviceClient<scavenger_hunt::huntBU>("visit_service");  
	ros::Subscriber sub0 = nh.subscribe("/camera/rgb/image_raw",1,save_image);

	//ros::Rate loop_rate(1);

	while(ros::ok()){

	    for (scavenger_hunt_msgs::Task& task : hunt.tasks) {
	    	sent=false;
	 		 std::string task_name = task.name;
	 		 temp_task = task;
	  		 std::string parameter = task.parameters[0].value;
	  			// See Task.msg for the full range of information available
			int time_task_start;  
			time_task_start = ros::Time::now().toSec(); 

			ROS_INFO("Shall I start executing this task?(y)");
			string x;
			cin >> x; // Get user input from the keyboard	 

			scavenger_hunt::huntBU srv;
			srv.request.loc_or_obj = parameter;  //seems like the term request is needed

			if (x=="y"){	
				task_execution_client.call(srv);
			    }

			ROS_INFO("Ready!");    
		    ROS_INFO("type y if the task is finished");

			string y;
			cin >> y; // Get user input from the keyboard

			if (y == "y"){
				proof.task_duration = ros::Time::now().toSec() - time_task_start;
						}

			
			ros::Rate(20).sleep();
	  		ros::spinOnce();

	  		//ros::spin();

		}
	
}
  return 0;
}