#define TDAXS12_ADDRESS 0x10
uint8_t RxData[31];
uint8_t RegisterAdd = 4;

void TDread() {
  read_ldrs_fast();
  if (ldr_front || ldr_back || ldr_left || ldr_right) return;
  TDread_noSkip();
}
void TDread_noSkip() {
  if (is_yellow && Look_at_goal) {
    target = yellow_angle;
    if (target > 180) target -= 360;
    zavie_robot = -target;
  } else {
    Wire.beginTransmission(TDAXS12_ADDRESS);
    Wire.write(RegisterAdd);
    Wire.endTransmission();
    Wire.requestFrom(TDAXS12_ADDRESS, 18);
    for (int i = 0; i < 18; i++) {
      read_ldrs_fast();
      if (ldr_front || ldr_back || ldr_left || ldr_right) return;
      RxData[i] = Wire.read();
    }
    int16_t Value = (int16_t)(RxData[0] << 8 | RxData[1]);
    zavie_robot = (Value / 100.0);
  }
}