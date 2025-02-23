#include <Wire.h>
#include <SoftwareSerial.h>
#include <Pixy2.h>

#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);

#define TDAXS12_ADDRESS 0x10
// #include <Pixy2.h>
// #include <Servo.h>
// #include <Pixy2SPI_SS.h>
#define rxPin PA2
#define txPin PA3
SoftwareSerial mySerial(rxPin, txPin);
#define rxPin PA10
#define txPin PA9
SoftwareSerial usbSerial(rxPin, txPin);
// HardwareSerial mySerial(rxPin, txPin);
#define TX_BUFFER_SIZE 100
#define RX_BUFFER_SIZE 500

#define BAUDRATE_9600 0
#define BAUDRATE_14400 1
#define BAUDRATE_19200 2
#define BAUDRATE_38400 3
#define BAUDRATE_56000 4
#define BAUDRATE_57600 5
#define BAUDRATE_115200 6
#define BAUDRATE_128000 7
#define BAUDRATE_256000 8

#define MAX_MOTOR_RPM 300
Pixy2 pixy;
int zavie_avalie = 0;
uint8_t RxData[31];
uint8_t RegisterAdd = 4;
int zavie_robot = 0;
// Pixy2SPI_SS pixy;
// Pixy2SPI_SS pixym;
// Servo myservo;
int xb, yb, ball_ang;
int xr = 159, yr = 115;
int servoPos = 90; 
int servoPos2;
uint16_t blocks;
int motor0, motor1, motor2, motor3;
char MOTOR_TX_Buffer[TX_BUFFER_SIZE];
volatile uint8_t MOTOR_RX_Buffer[RX_BUFFER_SIZE];
bool MOTOR_TxModeF = 0, MOTOR_ReceivingF = 0, MOTOR_NewPacketF = 0, MOTOR_SendDataToMasterF = 0;
uint8_t MOTOR_TX_AfterLastByteTimer = 0, MOTOR_RecTimer = 0;
uint8_t MOTOR_PacketCounter = 0, MOTOR_SyncStatus = 0, MOTOR_PacketLen = 0, MOTOR_ReceivedByte[50], MOTOR_DataCount = 0;
uint16_t MOTOR_LastTxDMACNT = 0, MOTOR_LastRxDMACNT = 0;
uint16_t MOTOR_RXDataCNT = 0, MOTOR_PacketError = 0;
int V = 400;
bool is_ball = false;
uint16_t SendCommandTimer = 0, Task10Ms = 0;
int adc[16], bat_alarm_time = 0;
float Vbat = 0;
uint8_t Incoming_ID = 0;
bool ldr_front = false, ldr_back = false, ldr_right = false, ldr_left = false;
#define LED1_ON digitalWrite(PB8, 1);
#define LED1_OFF digitalWrite(PB8, 0);
#define LED2_ON digitalWrite(PB10, 1);
#define LED2_OFF digitalWrite(PB10, 0);
#define Buzzer_ON digitalWrite(PB9, 1);
#define Buzzer_OFF digitalWrite(PB9, 0);
struct Motor {
  uint16_t Refball_angle;
  int16_t RefRPM;
  bool Enable;
  bool ResetEncoder;
  uint8_t TorquePercent, Feedback_TorquePercent;
  uint8_t Baudrate, Feedback_Baudrate;
  int16_t RealRPM;
  int32_t IncrementalEncoder;
  float AbsoluteEncoder;
  float Current;
  float Voltage;
} GTD35[20];

void setup() {
  pinMode(PB8, OUTPUT);   //LED1
  pinMode(PB10, OUTPUT);  //LED2
  pinMode(PB9, OUTPUT);   //Buzzer
  pinMode(PB9, OUTPUT);   //Buzzer
  pinMode(PB15, OUTPUT);  //Boost CTRL
  pinMode(PB12, OUTPUT);  //Shoot
  pinMode(PA4, OUTPUT);   //Spin
  pinMode(PA15, INPUT);   //SW1
  pinMode(PA0, OUTPUT);   //Mux S0
  pinMode(PC15, OUTPUT);  //Mux S1
  pinMode(PC14, OUTPUT);  //Mux S2
  pinMode(PC13, OUTPUT);  //Mux S3
  pinMode(PB0, INPUT);    //ADC input
  pinMode(PA11, INPUT);   //DIP1
  pinMode(PA12, INPUT);   //DIP2
  analogWrite(PB15, 10);

  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, 1);

  LED1_OFF;
  LED2_OFF;
  display.begin(0x2, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);
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

  for (int i = 0; i < 4; i++) {
    GTD35[i].Enable = 1;
    GTD35[i].Refball_angle = 400;
  }
  Wire.setSDA(PB7);
  Wire.setSCL(PB6);
  display.clearDisplay();
  display.display();
  pixy.init();
}
void loop() {
  digitalWrite(PB12, digitalRead(PA15));
  digitalWrite(PB8, digitalRead(PA15));
  // TDread();
  read_pixy();
  print_all();
  read_ldr();
  // if (is_ball) {
  //   if (ball_ang < 20 || ball_ang > 340) moveAngle(ball_ang);
  //   else if (ball_ang < 180) moveAngle(ball_ang + 40);
  //   else moveAngle(ball_ang - 40);
  // } else {
  //   motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
  // }
}
