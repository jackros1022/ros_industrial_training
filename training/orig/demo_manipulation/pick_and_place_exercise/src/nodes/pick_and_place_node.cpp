/*
 * Manipulation Lab
 * pick_and_place_node.cpp
 *
 *  Created on: May 21, 2013
 *      Author: ros developer 
 */

#include <pick_and_place_exercise/pick_and_place.h>

pick_and_place_config cfg;  // global var

// =============================== Main Thread ===============================
int main(int argc,char** argv)
{
  geometry_msgs::Pose box_pose;
  std::vector<geometry_msgs::Pose> pick_poses, place_poses;

  /* =========================================================================================*/
  /*	INITIALIZING ROS NODE
      Goal:
      - Observe all steps needed to properly initialize a ros node.
      - Look into the 'read_ros_parameters' function to take notice of the parameters that
        are available for the rest of the program. */
  /* =========================================================================================*/

  // ros initialization
  ros::init(argc,argv,"pick_and_place_node");
  ros::NodeHandle nh;
  tf::TransformListener tf_listener; // queries tf to find transforms
  ros::AsyncSpinner spinner(2);
  spinner.start();

  // reading parameters
  if(cfg.init())
  {
    ROS_INFO_STREAM("Parameters successfully read");
  }
  else
  {
    ROS_ERROR_STREAM("Parameters not found");
    return 0;
  }

  // moveit interface initialization
  move_group_interface::MoveGroup move_group(cfg.ARM_GROUP_NAME);

  // grasp action client initialization
  GraspActionClient grasp_action_client(cfg.GRASP_ACTION_SERVICE,true);

  // waiting to establish connections
  while(ros::ok() &&
      !grasp_action_client.waitForServer(ros::Duration(2.0f)))
  {
    ROS_INFO_STREAM("Waiting for servers");
  }


  /* ========================================*/

  move_to_wait_position(move_group);

  set_gripper(grasp_action_client, false);

  box_pose = detect_box_pick(tf_listener);

  pick_poses = create_pick_moves(tf_listener, box_pose);

  move_through_pick_poses(move_group,grasp_action_client,pick_poses);

  place_poses = create_place_moves(tf_listener);

  move_through_place_poses(move_group,grasp_action_client,place_poses);

  move_to_wait_position(move_group);

  return 0;
}
