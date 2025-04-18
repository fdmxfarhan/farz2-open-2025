#define TDAXS12_ADDRESS 0x10
uint8_t RxData[31];
uint8_t RegisterAdd = 4;

void TDread() {
  Wire.beginTransmission(TDAXS12_ADDRESS);
  Wire.write(RegisterAdd);
  Wire.endTransmission();
  Wire.requestFrom(TDAXS12_ADDRESS, 18);
  for (int i = 0; i < 18; i++) {
    RxData[i] = Wire.read();
  }
  int16_t Value = (int16_t)(RxData[0] << 8 | RxData[1]);
  zavie_robot = (Value / 100.0);
}