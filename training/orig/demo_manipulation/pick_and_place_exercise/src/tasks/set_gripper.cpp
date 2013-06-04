/*
 * open_gripper.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: ros-industrial
 */

#include <pick_and_place_exercise/pick_and_place.h>

void set_gripper(GraspActionClient& grasp_action_client, bool do_grasp)
{
  /*	SET GRIPPER
    Goal:
      - Use the grasp action client to open or close the gripper.
      - Confirm that the gripper was successfully opened or closed.  Exit program on failure;
    Hints:
		-
  Complete code below: */

  // task variables
  object_manipulation_msgs::GraspHandPostureExecutionGoal grasp_goal;
  bool success;

  // send grasp goal to open gripper
  if (do_grasp)
    grasp_goal.goal = object_manipulation_msgs::GraspHandPostureExecutionGoal::GRASP;
  else
    grasp_goal.goal = object_manipulation_msgs::GraspHandPostureExecutionGoal::RELEASE;

  /* Fill Code: [ use the 'sendGoal' method of the grasp client to open gripper] */
  grasp_action_client.sendGoal(grasp_goal);

  // confirm that gripper opened
  /* Fill Code: [ use the 'waitForResult' to check and save result in success variable] */
  success = grasp_action_client.waitForResult(ros::Duration(4.0f));

  if(success)
  {
    if (do_grasp)
      ROS_INFO_STREAM("Gripper closed");
    else
      ROS_INFO_STREAM("Gripper opened");
  }
  else
  {
    ROS_ERROR_STREAM("Gripper failure");

    /* Fill Code: [ call the ros shutdown function and then return] */
    exit(1);
  }
}


