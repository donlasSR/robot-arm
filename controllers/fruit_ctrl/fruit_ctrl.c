#include <webots/robot.h>
#include <webots/supervisor.h>
#include <webots/camera.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME_STEP 64
#define DROP_INTERVAL 10.0  // วินาทีที่ต้องรอก่อนปล่อยลูกต่อไป

int main(int argc, char **argv) {
  int apple_index = 0;
  int orange_index = 0;
  int redball_index = 0;
  int fruit_type = 1;

  const int max_apple = 10;
  const int max_orange = 10;
  const int max_redball = 10;

  char name[20];

  wb_robot_init();

  WbDeviceTag camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, TIME_STEP);

  WbNodeRef fruit;
  WbFieldRef fruit_trans_field;

  double fruit_initial_translation[3] = {0.57, 2.85, 0.34};

  double last_drop_time = wb_robot_get_time();  // เวลาล่าสุดที่ปล่อยลูก

  while (wb_robot_step(TIME_STEP) != -1) {
    double current_time = wb_robot_get_time();

    // เริ่มปล่อยหลังจากเวลาเริ่มต้น (เช่น 7.5 วินาที)
    if (current_time > 1.5 && current_time - last_drop_time >= DROP_INTERVAL) {
      if (fruit_type > 0) {
        // เลือกชนิดผลไม้แบบสุ่ม (1=apple, 2=orange, 3=redball)
        fruit_type = 1 + (rand() % 3);

        // ตรวจสอบไม่ให้เกิน
        if (apple_index >= max_apple && orange_index >= max_orange)
          fruit_type = 3;
        else if (apple_index >= max_apple && fruit_type == 1)
          fruit_type = (rand() % 2) ? 2 : 3;
        else if (orange_index >= max_orange && fruit_type == 2)
          fruit_type = (rand() % 2) ? 1 : 3;
        else if (redball_index >= max_redball && fruit_type == 3)
          fruit_type = (rand() % 2) ? 1 : 2;

        // ตั้งชื่อ
        if (fruit_type == 1 && apple_index < max_apple) {
          sprintf(name, "apple%d", apple_index++);
        } else if (fruit_type == 2 && orange_index < max_orange) {
          sprintf(name, "orange%d", orange_index++);
        } else if (fruit_type == 3 && redball_index < max_redball) {
          sprintf(name, "redball%d", redball_index++);
        } else {
          continue;
        }

        // ปล่อยวัตถุ
        fruit = wb_supervisor_node_get_from_def(name);
        if (fruit) {
          fruit_trans_field = wb_supervisor_node_get_field(fruit, "translation");
          if (fruit_trans_field) {
            wb_supervisor_field_set_sf_vec3f(fruit_trans_field, fruit_initial_translation);
          } else {
            printf("ไม่พบ field 'translation' ในวัตถุ %s\n", name);
          }
        } else {
          printf("ไม่พบวัตถุชื่อ %s\n", name);
        }

        // บันทึกเวลาที่ปล่อยล่าสุด
        last_drop_time = current_time;

        // ตรวจสอบว่าหมดหรือยัง
        if (apple_index == max_apple && orange_index == max_orange && redball_index == max_redball)
          fruit_type = 0;
      }
    }
  }

  wb_robot_cleanup();
  return 0;
}
