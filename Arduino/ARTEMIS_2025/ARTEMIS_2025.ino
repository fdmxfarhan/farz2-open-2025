#include <Wire.h>
#include <SoftwareSerial.h>
// #include <Pixy2.h>
#include <Adafruit_SH1106_STM32.h>
#include <Pixy2I2C.h>

#define rxPin1 PA2
#define txPin1 PA3
#define rxPin2 PA10
#define txPin2 PA9

#define LED1_ON digitalWrite(PB8, 1);
#define LED1_OFF digitalWrite(PB8, 0);
#define LED2_ON digitalWrite(PB10, 1);
#define LED2_OFF digitalWrite(PB10, 0);
#define Buzzer_ON digitalWrite(PB9, 1);
#define Buzzer_OFF digitalWrite(PB9, 0);

#define robot1_x 152
#define robot1_y 115
#define robot2_x 152
#define robot2_y 115

Adafruit_SH1106 display(-1);
SoftwareSerial mySerial(rxPin1, txPin1);
SoftwareSerial usbSerial(rxPin2, txPin2);
// Pixy2 pixy;
Pixy2I2C pixy;


int V = 400;
int zavie_robot = 0, robot_id;
int xb, yb, ball_ang, ball_dist;
int xr = 152, yr = 115;
int adc[16], bat_alarm_time = 0;
int shoot_cnt = 0, pixy_timeout_cnt = 0;
float Vbat = 0;
bool already_shooted = false;
bool is_ball = false, ball_in_kicker = false;
bool ldr_front = false, ldr_back = false, ldr_right = false, ldr_left = false;


// #define PWM_FREQUENCY 500000  // 10 kHz (adjust as needed)
// #define PWM_RESOLUTION 1000  // 1000 steps for duty cycle control

void setup() {
  pinMode(PB8, OUTPUT);     //LED1
  pinMode(PB10, OUTPUT);    //LED2
  pinMode(PB9, OUTPUT);     //Buzzer
  pinMode(PB9, OUTPUT);     //Buzzer
  pinMode(PB15, OUTPUT);    //Boost Control
  pinMode(PB12, OUTPUT);    //Kicker
  pinMode(PA4, OUTPUT);     //Dribler
  pinMode(PA15, INPUT);     //SW1
  pinMode(PA0, OUTPUT);     //Mux S0
  pinMode(PC15, OUTPUT);    //Mux S1
  pinMode(PC14, OUTPUT);    //Mux S2
  pinMode(PC13, OUTPUT);    //Mux S3
  pinMode(PB0, INPUT);      //ADC input
  pinMode(PA11, INPUT);     //DIP1
  pinMode(PA12, INPUT);     //DIP2
  pinMode(rxPin1, INPUT);   //rx motor
  pinMode(txPin1, OUTPUT);  //tx motor
  pinMode(rxPin2, INPUT);   //rx USB
  pinMode(txPin2, OUTPUT);  //tx USB
  pinMode(PB5, INPUT);      //Robot ID

  if (digitalRead(PB5)) {
    robot_id = 1;
    xr = robot1_x;
    yr = robot1_y;
  } else {
    robot_id = 2;
    xr = robot2_x;
    yr = robot2_y;
  }

  analogWrite(PB15, 10);
  // HardwareTimer *MyTim = new HardwareTimer(TIM1);
  // MyTim->setPrescaleFactor(1);  // No prescaler
  // MyTim->setOverflow(1000);  // Set the timer overflow value for resolution
  // MyTim->setMode(3, TIMER_OUTPUT_COMPARE_PWM1, PB15);  // Use PA10 for TIM1_CH3 (check your board's pinout)
  // MyTim->setCaptureCompare(3, 100, TICK_COMPARE_FORMAT);  // Start with 50% duty cycle
  // MyTim->resume();
  // TIM1->CCR3 = 500;

  display.begin(0x2, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(6);
  display.clearDisplay();
  display.setCursor(50, 10);
  display.print(robot_id);
  display.display();
  for (int i = 0; i < 2; i++) {
    LED1_ON;
    Buzzer_ON;
    delay(50);
    LED1_OFF;
    Buzzer_OFF;
    delay(50);
  }
  delay(2000);
  mySerial.begin(115200);
  usbSerial.begin(115200);

  init_motors();
  Wire.setSDA(PB7);
  Wire.setSCL(PB6);
  display.clearDisplay();
  display.setTextSize(1);
  display.display();
  pixy.init();
}
void loop() {
  TDread();
  read_pixy();
  read_ldr();
  print_all();
  // print_adc();
  Forward();
}
