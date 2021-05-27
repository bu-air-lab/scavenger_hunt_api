#!/usr/bin/env python


import subprocess
from scavenger_hunt.srv import AddTwoInts,AddTwoIntsResponse
import rospy

def handle_visit_location(location_name):
 
	print ('inside handle')
	proc = subprocess.Popen(["rosrun","bwi_tasks", "go_to_door_node",
							"_door:=shiqi_office_d"],stdout=subprocess.PIPE)

	out, err = proc.communicate()
	print (out)
	return 1

def main():
	rospy.init_node('visit_server')
	s = rospy.Service('visit_service', AddTwoInts, handle_visit_location)
	print("Ready spin")
	print('Make robot is booted up and localized')
	rospy.spin()

	
	#visit_location('shiqi_office')



if __name__ == '__main__':
	main()