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
int x, y, ball_ang;
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

uint8_t Incoming_ID = 0;
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
  analogWrite(PB15, 10);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(PC13, OUTPUT);

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
  // pixy.init();
  mySerial.begin(115200);
  usbSerial.begin(115200);
  while (!mySerial) delay(100);

  for (int i = 0; i < 4; i++) {
    GTD35[i].Enable = 1;
    GTD35[i].Refball_angle = 400;
  }
  Wire.setSDA(PB7);
  Wire.setSCL(PB6);
  // Wire.begin();
  // TDread();
  display.clearDisplay();
  display.display();
}
void loop() {
  motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
  digitalWrite(PB12, digitalRead(PA15));
  digitalWrite(PB8, digitalRead(PA15));
  TDread();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Gyro:");
  display.print(zavie_robot);
  display.display();
  // int i;
  // blocks = pixy.ccc.getBlocks();

  // is_ball = false;
  // if (blocks) {
  //   for (i = 0; i < pixy.ccc.numBlocks; i++) {
  //     if (pixy.ccc.blocks[i].m_signature == 1) {
  //       is_ball = true;
  //       x = pixy.ccc.blocks[i].m_x - 160;
  //       y = pixy.ccc.blocks[i].m_y - 123;
  //       ball_ang = atan2(x, y) * 180 / PI;
  //       if (ball_ang < 0) ball_ang += 360;
  //       usbSerial.print("x:");
  //       usbSerial.print(x);
  //       usbSerial.print("y:");
  //       usbSerial.print(y);
  //       usbSerial.print("ball_ang:");
  //       usbSerial.println(ball_ang);
  //     }
  //   }
  // }
  // if (is_ball) {
  //   if (ball_ang < 10 || ball_ang > 350) moveAngle(ball_ang);
  //   else if (ball_ang < 180) moveAngle(ball_ang + 90);
  //   else moveAngle(ball_ang - 90);
  // } else {
  //   motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
  // }
}

void TDread() {
  Wire.beginTransmission(TDAXS12_ADDRESS);
  Wire.write(RegisterAdd);
  Wire.endTransmission();
  Wire.requestFrom(TDAXS12_ADDRESS, 18);
  for (int i = 0; i < 18; i++) {
    RxData[i] = Wire.read();
  }
  int16_t Value = (int16_t)(RxData[0] << 8 | RxData[1]);
  zavie_robot = (Value / 100.0) - zavie_avalie;
}
void Motor_SetID(uint8_t PreviousID, uint8_t NewID) {
  MOTOR_TX_Buffer[0] = 0xAA;
  MOTOR_TX_Buffer[1] = 0xAA;
  MOTOR_TX_Buffer[2] = 0;
  MOTOR_TX_Buffer[3] = PreviousID;
  MOTOR_TX_Buffer[4] = 0x11;   //Command
  MOTOR_TX_Buffer[5] = NewID;  //Data

  MOTOR_TX_Buffer[6] = Checksum(&MOTOR_TX_Buffer[2], 4);

  MOTOR_SendData(7);
}
void MOTOR_SendData(int length) {
  for (int q = 0; q < length; q++) {
    mySerial.write(MOTOR_TX_Buffer[q]);
    // Serial.write(MOTOR_TX_Buffer[q]);
  }
  MOTOR_TxModeF = 1;
}
uint8_t Checksum(char *Buf, uint16_t Length) {
  uint8_t CheckByte = 0;

  for (uint16_t i = 0; i < Length; i++) {
    CheckByte ^= *(Buf++);
  }
  return CheckByte;
}
void MotorMove(uint8_t NumOfMotor) {
  MOTOR_TX_Buffer[0] = 0xAA;
  MOTOR_TX_Buffer[1] = 0xAA;
  MOTOR_TX_Buffer[2] = NumOfMotor;

  uint16_t StartDataAdd = NumOfMotor + 3;

  for (uint8_t i = 0; i < NumOfMotor; i++) {
    int16_t Speed = GTD35[i].RefRPM;
    uint8_t Speed_MSB = (Speed >> 8) & 0xFF;
    uint8_t Speed_LSB = Speed & 0xFF;

    uint16_t ball_angle = GTD35[i].Refball_angle * 10;
    uint8_t ball_angle_MSB = (ball_angle >> 8) & 0xFF;
    uint8_t ball_angle_LSB = ball_angle & 0xFF;

    uint8_t ControlByte = 0;
    if (GTD35[i].Enable) ControlByte |= 0x0F;
    if (GTD35[i].ResetEncoder) ControlByte |= 0xF0;

    MOTOR_TX_Buffer[i + 3] = i;
    MOTOR_TX_Buffer[StartDataAdd] = ControlByte;
    MOTOR_TX_Buffer[StartDataAdd + 1] = Speed_MSB;
    MOTOR_TX_Buffer[StartDataAdd + 2] = Speed_LSB;
    MOTOR_TX_Buffer[StartDataAdd + 3] = ball_angle_MSB;
    MOTOR_TX_Buffer[StartDataAdd + 4] = ball_angle_LSB;

    StartDataAdd += 5;
  }

  MOTOR_TX_Buffer[StartDataAdd] = Checksum(&MOTOR_TX_Buffer[2], StartDataAdd - 2);

  MOTOR_SendData(StartDataAdd + 1);
}
void motor(int zero, int one, int two, int three) {
  GTD35[0].Enable = 1;
  GTD35[1].Enable = 1;
  GTD35[2].Enable = 1;
  GTD35[3].Enable = 1;
  GTD35[0].RefRPM = zero;
  GTD35[0].Refball_angle = 400;
  GTD35[1].RefRPM = one;
  GTD35[1].Refball_angle = 400;
  GTD35[2].RefRPM = two;
  GTD35[2].Refball_angle = 400;
  GTD35[3].RefRPM = three;
  GTD35[3].Refball_angle = 400;
  MotorMove(4);
}
void moveAngle(int Ang) {
  int vx = V * sin(Ang * PI / 180);
  int vy = V * cos(Ang * PI / 180);
  motor((vy - vx) / 2 + zavie_robot, (vy + vx) / 2 + zavie_robot, (-vy + vx) / 2 + zavie_robot, (-vy - vx) / 2 + zavie_robot);
}
