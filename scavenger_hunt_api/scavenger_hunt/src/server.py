#!/usr/bin/env python


import subprocess
from scavenger_hunt.srv import huntBU
import rospy

def handle_visit_location(req):
 
	print ('Inside handle')
	print ('Be patient, task planner takes a while (15s) to start')
	# Spawning a process to run the task planner
	proc = subprocess.Popen(["rosrun","bwi_tasks", "go_to_door_node",
							"_door:="+req.loc_or_obj],stdout=subprocess.PIPE)

	out, err = proc.communicate()
	print (out)
	return 1

def main():
	rospy.init_node('visit_server')
	s = rospy.Service('visit_service', huntBU, handle_visit_location)
	print("Ready spin")
	print('Make robot is booted up and localized')
	rospy.spin()




if __name__ == '__main__':
	main()