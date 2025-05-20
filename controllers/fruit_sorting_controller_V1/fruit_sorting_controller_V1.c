/*
 * File:          fruit_sorting_controller_V2.c
 */

#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <webots/position_sensor.h>
#include <webots/camera.h>
#include <webots/camera_recognition_object.h>
#include <webots/motor.h>
#include <webots/supervisor.h>

#include <stdio.h>
#include <string.h>

#define TIME_STEP 32

enum State {WAITING, PICKING, ROTATING, DROPPING, ROTATE_BACK};

int main(int argc, char **argv) {
  wb_robot_init();
  
  int counter = 0, i = 0;
  int state = WAITING;
  
  const double target_positions[3][5] = {
    {-1.570796, -1.87972, -2.139774, -2.363176, -1.50971},  // orange
    {0.2, -1.87972, -2.139774, -2.363176, -1.50971},                         // apple
    {2, -1.87972, -2.139774, -2.363176, -1.50971}
  };
  
  double speed = 2.0;
  int model = 0;  // 0 = orange, 1 = apple, 2 = red_ball
  char fruit[20];
  int apple = 0;
  int orange = 0;
  int red_ball = 0;

  char strP[100];

  if (argc == 2)
    sscanf(argv[1], "%lf", &speed);

  // Gripper fingers
  WbDeviceTag hand_motors[3];
  hand_motors[0] = wb_robot_get_device("finger_1_joint_1");
  hand_motors[1] = wb_robot_get_device("finger_2_joint_1");
  hand_motors[2] = wb_robot_get_device("finger_middle_joint_1");

  // UR arm joints
  WbDeviceTag ur_motors[5];
  ur_motors[0] = wb_robot_get_device("shoulder_pan_joint");
  ur_motors[1] = wb_robot_get_device("shoulder_lift_joint");
  ur_motors[2] = wb_robot_get_device("elbow_joint");
  ur_motors[3] = wb_robot_get_device("wrist_1_joint");
  ur_motors[4] = wb_robot_get_device("wrist_2_joint");

  // Camera setup
  WbDeviceTag camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, 2 * TIME_STEP);
  wb_camera_recognition_enable(camera, 2 * TIME_STEP);
  
  for (i = 0; i < 5; ++i)
    wb_motor_set_velocity(ur_motors[i], speed);

  WbDeviceTag distance_sensor = wb_robot_get_device("distance sensor");
  wb_distance_sensor_enable(distance_sensor, TIME_STEP);

  WbDeviceTag position_sensor = wb_robot_get_device("wrist_1_joint_sensor");
  wb_position_sensor_enable(position_sensor, TIME_STEP);

  while (wb_robot_step(TIME_STEP) != -1) {
    int number_of_objects = wb_camera_recognition_get_number_of_objects(camera);
    const WbCameraRecognitionObject *objects = wb_camera_recognition_get_objects(camera);

    // ตรวจจับผลไม้ (ไม่เพิ่ม count ที่นี่)
    if (number_of_objects > 0) {
      sprintf(fruit, "%s", objects[0].model);
      if (strcmp(fruit, "apple") == 0)
        model = 1;
      else if (strcmp(fruit, "redball") == 0)
        model = 2;
      else
        model = 0;
    }
    printf("Detected model: %s\n", fruit);

    // FSM
    if (counter <= 0) {
      switch (state) {
        case WAITING:
          if (wb_distance_sensor_get_value(distance_sensor) < 500) {
            state = PICKING;
            counter = 8;
            for (i = 0; i < 3; ++i)
              wb_motor_set_position(hand_motors[i], 0.52); // close gripper
          }
          break;

        case PICKING:
          for (i = 0; i < 5; ++i)
            wb_motor_set_position(ur_motors[i], target_positions[model][i]);
          state = ROTATING;
          break;

        case ROTATING:
          if (wb_position_sensor_get_value(position_sensor) < -2.05) {
            counter = 8;
            state = DROPPING;
            // เพิ่มการนับจำนวนหลังจากปล่อยผลไม้
          if (model == 0)
            orange++;
            else if (model == 1)
              apple++;
            else if (model == 2)
              red_ball++;
            for (i = 0; i < 3; ++i)
              wb_motor_set_position(hand_motors[i], wb_motor_get_min_position(hand_motors[i])); // open gripper
          }
          
          break;

        case DROPPING:
          

          for (i = 0; i < 5; ++i)
            wb_motor_set_position(ur_motors[i], 0.0); // return to initial
          state = ROTATE_BACK;
          break;

        case ROTATE_BACK:
          if (wb_position_sensor_get_value(position_sensor) > -0.1) {
            state = WAITING;
          }
          break;
      }
    }
    counter--;

    // แสดงจำนวนผลไม้
    sprintf(strP, "Oranges: %d", orange);
    wb_supervisor_set_label(0, strP, 0.45, 0.96, 0.06, 0xffaa00, 0, "Lucida Console");
    
    sprintf(strP, "Apples : %d", apple);
    wb_supervisor_set_label(1, strP, 0.3, 0.96, 0.06, 0x55ff55, 0, "Lucida Console");

    sprintf(strP, "redballs: %d", red_ball);
    wb_supervisor_set_label(2, strP, 0.15, 0.96, 0.06, 0xff5555, 0, "Lucida Console");
  }

  printf("=== Controller started ===\n");

  wb_robot_cleanup();
  return 0;
}
