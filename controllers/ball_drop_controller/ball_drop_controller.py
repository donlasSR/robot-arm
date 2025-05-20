from controller import Robot, Supervisor

robot = Supervisor()
timestep = int(robot.getBasicTimeStep())

# ตั้งค่ารายชื่อของลูกบอลทั้งหมด
ball_names = [f"ball_{i}" for i in range(1, 21)]

# ตั้งค่าเวลาเริ่มต้น และคาบเวลาที่จะปล่อยลูกถัดไป (2 วินาที)
start_time = robot.getTime()
drop_interval = 2.0
next_drop_time = start_time + drop_interval

current_ball_index = 0

while robot.step(timestep) != -1:
    current_time = robot.getTime()

    if current_ball_index < len(ball_names) and current_time >= next_drop_time:
        ball_name = ball_names[current_ball_index]
        ball_node = robot.getFromDef(ball_name)

        if ball_node:
            # ปรับตำแหน่ง Z ของลูกบอลให้ลอยพ้นกล่องเล็กน้อย (เช่น +0.02 m)
            translation_field = ball_node.getField("translation")
            position = translation_field.getSFVec3f()
            position[1] -= 0.02  # ขยับแกน Y ลงเพื่อให้ลูกบอลตกลงมา
            translation_field.setSFVec3f(position)

            print(f"Dropped: {ball_name} at t={current_time:.2f}s")
        else:
            print(f"Ball {ball_name} not found!")

        current_ball_index += 1
        next_drop_time = current_time + drop_interval
