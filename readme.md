#robot-arm
#A robotic arm model for loading objects into a

โปรเจกต์นี้เป็นการจำลองหุ่นยนต์ใน Webots ที่สามารถตรวจจับผลไม้ (แอปเปิล, ส้ม, ลูกบอลแดง) โดยใช้กล้อง และควบคุมแขนกลเพื่อแยกแยะวัตถุลงในกล่องที่ถูกต้องโดยอัตโนมัติ พร้อมทั้งมีระบบ Supervisor ปล่อยผลไม้อัตโนมัติลงบนสายพาน

## 🛠 Features

- ตรวจจับวัตถุด้วยกล้อง (Image Classification)
- ควบคุมแขนกล UR5 สำหรับหยิบ-วางวัตถุ
- จำแนกวัตถุเป็น 3 ประเภท: Apple, Orange, Red Ball
- ระบบ Supervisor ปล่อยวัตถุบนสายพานแบบอัตโนมัติทุก 2 วินาที
- เขียนด้วยภาษา C และ Python

## 🧰 เทคโนโลยีที่ใช้

- [Webots](https://cyberbotics.com/) - Robot simulator
- UR5 Robot Arm (Simulated)
- กล้องจำลอง (Webots Camera)
- Python (สำหรับ Supervisor)
- C (สำหรับหุ่นยนต์)

## 🗂 โครงสร้างโปรเจกต์
niversal Robot V2/
├── controllers/
│ ├── ur_robot/ # ตัวควบคุมหุ่นยนต์ (ภาษา C)
│ └── supervisor/ # Supervisor (ภาษา Python)
├── protos/
│ └── ... # โปรโตไทป์ของหุ่นยนต์หรือวัตถุ
├── worlds/
│ └── universal_robot_v2.wbt # ฉากจำลองหลัก
└── README.md

## 🚀 วิธีใช้งาน

1. ติดตั้ง Webots [ดาวน์โหลดที่นี่](https://cyberbotics.com/#download)
2. เปิดโปรเจกต์ใน Webots โดยเลือกไฟล์ `universal_robot_v2.wbt` ในโฟลเดอร์ `worlds/`
3. รัน simulation แล้วหุ่นยนต์จะเริ่มทำงานอัตโนมัติ

## 📷 ตัวอย่างการทำงาน

> *ใส่ภาพหรือวิดีโอประกอบที่นี่ (หากมี)*

## 🙋‍♂️ ผู้พัฒนา

- จอนนี่ (Jonnie)
- สนับสนุนโดย ChatGPT (OpenAI)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
