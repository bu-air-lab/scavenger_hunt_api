#!/usr/bin/env python


import subprocess
from scavenger_hunt.srv import AddTwoInts,AddTwoIntsResponse
import rospy

def handle_visit_location(location_name):
 
	print ('inside handle')
	#proc = subprocess.Popen(["rosrun","bwi_tasks", "go_to_location_node",
	#						"_location:="+location_name],stdout=subprocess.PIPE)

	#out, err = proc.communicate()
	#print (out)
	return 1

def add_two_ints_server():
	pass

def main():
	rospy.init_node('visit_server')
	s = rospy.Service('visit_service', AddTwoInts, handle_visit_location)
	print("Ready dpin")
	rospy.spin()

	print('Make robot is booted up')
	#visit_location('shiqi_office')



if __name__ == '__main__':
	main()