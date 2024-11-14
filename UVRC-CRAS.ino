//Khai báo thư viện
#include <Adafruit_PWMServoDriver.h>
#include <PS2X_lib.h>
#include <Wire.h>

//Class
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
PS2X ps2x;

//Define PS2
#define PS2_DAT 12 // MISO 
#define PS2_CMD 13 // MOSI 
#define PS2_SEL 15 // SS 
#define PS2_CLK 14 // SLK
#define pressures false
#define rumble false

// Define Motor DC 12V - 2 bánh traction (D: Dương, A: Âm, T: Trái, P: Phải)
#define DT 8 
#define AT 9 
#define DP 14
#define AP 15

// Define Motor DC 12V - Chổi thu bóng (L: Lấy)
#define DL 10
#define AL 11


//Define Servo 360 (SV: Servo)
#define SVT 1
#define SVP 2



//Tạo các biến cần thiết
bool thu = false;
int speed = 3071;
bool boost = false;

void setup() {

  // Khởi tạo Serial Monitor
  Serial.begin(115200); 

  // Khởi tạo VIA B
  pwm.begin(); 
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Wire.setClock(400000);
  
  //Kiểm tra lỗi PS2
  int error = -1; 
  for (int i = 0; i < 10; i++)
  {
    delay(1000);
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble); 
    Serial.print("."); 
    if(!error) 
      break; 
  }
}


void loop() {
  ps2x.read_gamepad(false, false);

//Di chuyển
  if (ps2x.Button(PSB_PAD_UP)) {  // Tiến lên
    pwm.setPWM(DT, 0, speed);
    pwm.setPWM(AT, 0, 0);
    pwm.setPWM(DP, 0, speed);
    pwm.setPWM(AP, 0, 0);
    Serial.println("Đang tiến");
    delay(50); 
  }

  else if (ps2x.Button(PSB_PAD_DOWN)) {  // Lùi xuống
    pwm.setPWM(DT, 0, 0);
    pwm.setPWM(AT, 0, speed);
    pwm.setPWM(DP, 0, 0);
    pwm.setPWM(AP, 0, speed);
    Serial.println("Đang lùi");
    delay(50); 
  }

  else if (ps2x.Button(PSB_PAD_LEFT)) {  // Sang trái
    pwm.setPWM(DT, 0, 0);
    pwm.setPWM(AT, 0, 2048);
    pwm.setPWM(DP, 0, 2048);
    pwm.setPWM(AP, 0, 0);
    Serial.println("Đang sang trái");
    delay(50); 
  }

  else if (ps2x.Button(PSB_PAD_RIGHT)) {  // Sang phải
    pwm.setPWM(DT, 0, 2048);
    pwm.setPWM(AT, 0, 0);
    pwm.setPWM(DP, 0, 0);
    pwm.setPWM(AP, 0, 2048);
    Serial.println("Đang sang phải");
    delay(50); 
  }

  else if (ps2x.ButtonReleased(PSB_PAD_UP) || ps2x.ButtonReleased(PSB_PAD_DOWN) || ps2x.ButtonReleased(PSB_PAD_LEFT) || ps2x.ButtonReleased(PSB_PAD_RIGHT)) { //Dừng
    pwm.setPWM(DT, 0, 0);
    pwm.setPWM(AT, 0, 0);
    pwm.setPWM(DP, 0, 0);
    pwm.setPWM(AP, 0, 0);
    Serial.println("Đã dừng");
    delay(50);
  }




  //Tăng tốc
  if (ps2x.ButtonPressed(PSB_L2)) {
    if (boost == false) {
      speed = 4095;
      boost = true;
      Serial.println("Tốc hành");
      delay(50);
    }

    else {
      speed = 3071;
      boost = false;
      Serial.println("Tốc độ bình thường");
      delay(50);
    }
  }




  //Thu bóng
 if (ps2x.ButtonPressed(PSB_R1)) {
    if (thu == false) {
      pwm.setPWM(DL, 0, 3686);
      thu = true;
      Serial.println("Thu bóng");
      delay(50);
    }

    else {
      pwm.setPWM(DL, 0, 0);
      thu = false;
      Serial.println("Không quay");
      delay(50);
    }
  }




  //Nhả bóng
 if (ps2x.ButtonPressed(PSB_R2)) {
    if (thu == false) {
      pwm.setPWM(AL, 0, 3686);
      thu = true;
      Serial.println("Nhả bóng");
      delay(50);
    }

    else {
      pwm.setPWM(AL, 0, 0);
      thu = false;
      Serial.println("Không quay");
      delay(50);
    }
  }




  //Nâng thùng
  if (ps2x.Button(PSB_RED)) {
      pwm.writeMicroseconds(SVT, 700);
      pwm.writeMicroseconds(SVP, 700);
      Serial.println("Nâng thùng")
      delay(50);
    }

    else if (ps2x.ButtonReleased(PSB_RED)) {
      pwm.writeMicroseconds(SVT, 1400);
      pwm.writeMicroseconds(SVP, 1400);
      Serial.println("Không di chuyển");
      delay(50);
  }



  //Hạ thùng
  if (ps2x.Button(PSB_GREEN)) {
      pwm.writeMicroseconds(SVT, 2300);
      pwm.writeMicroseconds(SVP, 2300);
      Serial.println("Hạ thùng")
      delay(50);
    }

    else if (ps2x.ButtonReleased(PSB_GREEN)) {
      pwm.writeMicroseconds(SVT, 1400);
      pwm.writeMicroseconds(SVP, 1400);
      Serial.println("Không di chuyển");
      delay(50);
    }
}
/* CODE THUỘC VỀ ĐỘI THI CRAS*/
