#define TX_BUFFER_SIZE 100
#define RX_BUFFER_SIZE 500

char MOTOR_TX_Buffer[TX_BUFFER_SIZE];
volatile uint8_t MOTOR_RX_Buffer[RX_BUFFER_SIZE];
bool MOTOR_TxModeF = 0, MOTOR_ReceivingF = 0, MOTOR_NewPacketF = 0, MOTOR_SendDataToMasterF = 0;

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

void init_motors() {
  for (int i = 0; i < 4; i++) {
    GTD35[i].Enable = 1;
    GTD35[i].Refball_angle = 400;
  }
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
    delayMicroseconds(10);
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

void move4sec(int a, int sec) {
  for (int i = 0; i < sec; i++) {
    TDread();
    read_cam();
    read_ldr();
    if (digitalRead(PA11)) print_adc();
    else print_all();
    moveAngle(a);
  }
}