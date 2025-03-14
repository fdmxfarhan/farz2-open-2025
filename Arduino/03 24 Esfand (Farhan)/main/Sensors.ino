uint16_t blocks;
#define DIP2 digitalRead(PA12)

void read_pixy() {
  int i;
  blocks = pixy.ccc.getBlocks();

  if (blocks) {
    for (i = 0; i < pixy.ccc.numBlocks; i++) {
      if (pixy.ccc.blocks[i].m_signature == 1) {
        is_ball = true;
        xb = pixy.ccc.blocks[i].m_x;
        yb = pixy.ccc.blocks[i].m_y;
        ball_ang = atan2(xb - xr, yb - yr) * 180 / PI;
        ball_dist = sqrt(pow(xb - xr, 2) + pow(yb - yr, 2));
        if (ball_ang < 0) ball_ang += 360;
      }
    }
    pixy_timeout_cnt = 0;
  } else {
    is_ball = false;
  }
}
void read_ldrs_fast(){
  for (int i = 0; i <= 6; i+=2) {
    digitalWrite(PA0, (i / 1) % 2);
    digitalWrite(PC15, (i / 2) % 2);
    digitalWrite(PC14, (i / 4) % 2);
    digitalWrite(PC13, (i / 8) % 2);
    adc[i] = analogRead(PB0);
  }
  int LDR_SENS = 1000;
  if (!ldr_front && !ldr_right && !ldr_left && !ldr_back) {
    if (adc[2] > LDR_SENS) ldr_right = true;
    else if (adc[6] > LDR_SENS) ldr_left = true;
    else if (adc[0] > LDR_SENS) ldr_front = true;
    else if (adc[4] > LDR_SENS) ldr_back = true;
  }
}
void read_ldr() {
  digitalWrite(PB12, digitalRead(PA15));
  digitalWrite(PB8, digitalRead(PA15));
  for (int i = 0; i < 16; i++) {
    digitalWrite(PA0, (i / 1) % 2);
    digitalWrite(PC15, (i / 2) % 2);
    digitalWrite(PC14, (i / 4) % 2);
    digitalWrite(PC13, (i / 8) % 2);
    adc[i] = analogRead(PB0);
  }
  int LDR_SENS = 1000;
  if (!ldr_front && !ldr_right && !ldr_left && !ldr_back) {
    if (adc[2] > LDR_SENS) ldr_right = true;
    else if (adc[6] > LDR_SENS) ldr_left = true;
    else if (adc[0] > LDR_SENS) ldr_front = true;
    else if (adc[4] > LDR_SENS) ldr_back = true;
  }

  /////// Battery Voltage Alarm
  Vbat = adc[13] / 18.0 - 9.7;
  // if (Vbat < 9 && Vbat > 5) {
  //   if (millis() - bat_alarm_time < 200) {
  //     digitalWrite(PB9, 1);
  //   } else if (millis() - bat_alarm_time < 2000) {
  //     digitalWrite(PB9, 0);
  //   } else bat_alarm_time = millis();
  // } else digitalWrite(PB9, 0);

  //////
  if (robot_id == 1) {
    if (adc[11] > 500) ball_in_kicker = true;
    else {
      ball_in_kicker = false;
      already_shooted = false;
    }
  } else {
    if (adc[11] > 500) ball_in_kicker = true;
    else {
      ball_in_kicker = false;
      already_shooted = false;
    }
  }

  /////////////// Voltage Boost Control
  if (DIP2) {
    if (adc[15] > 900) {
      LED2_ON;
      analogWrite(PB15, 0);
    }
    if (adc[15] < 800) {
      LED2_OFF;
      analogWrite(PB15, 20);
    }
  } else {
    if (adc[15] > 500) {
      LED2_ON;
      analogWrite(PB15, 0);
    }
    if (adc[15] < 400) {
      LED2_OFF;
      analogWrite(PB15, 20);
    }
  }
}