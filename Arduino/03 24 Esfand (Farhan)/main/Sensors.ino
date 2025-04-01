uint16_t blocks;
#define DIP2 digitalRead(PA12)

void read_pixy() {
  int i;
  blocks = pixy.ccc.getBlocks();
  is_ball = false;
  is_yellow = false;
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
      if (pixy.ccc.blocks[i].m_signature == 2) {
        is_yellow = true;
        yellow_x = pixy.ccc.blocks[i].m_x;
        yellow_y = pixy.ccc.blocks[i].m_y;
        yellow_angle = atan2(yellow_x - xr, yellow_y - yr) * 180 / PI;
        ball_dist = sqrt(pow(yellow_x - xr, 2) + pow(yellow_y - yr, 2));
        if (yellow_angle < 0) yellow_angle += 360;
      }
    }
    pixy_timeout_cnt = 0;
  }
}
void read_ldrs_fast() {
  for (int i = 0; i <= 6; i += 2) {
    digitalWrite(PA0, (i / 1) % 2);
    digitalWrite(PC15, (i / 2) % 2);
    digitalWrite(PC14, (i / 4) % 2);
    digitalWrite(PC13, (i / 8) % 2);
    adc[i] = analogRead(PB0);
  }
  int LDR_SENS = 950;
  if (!ldr_front && !ldr_right && !ldr_left && !ldr_back) {
    if (robot_id == 1) {
      if (adc[2] > LDR_SENS) ldr_right = true;
      else if (adc[6] > LDR_SENS) ldr_left = true;
      else if (adc[0] > LDR_SENS) ldr_front = true;
      else if (adc[4] > LDR_SENS) ldr_back = true;
    } else {
      if (adc[2] > LDR_SENS) ldr_right = true;
      else if (adc[6] > LDR_SENS) ldr_left = true;
      else if (adc[0] > LDR_SENS) ldr_front = true;
      else if (adc[4] > LDR_SENS) ldr_back = true;
    }
  }
}
void read_sensors() {
  digitalWrite(PB12, digitalRead(PA15));
  digitalWrite(PB8, digitalRead(PA15));
  for (int i = 0; i < 16; i++) {
    digitalWrite(PA0, (i / 1) % 2);
    digitalWrite(PC15, (i / 2) % 2);
    digitalWrite(PC14, (i / 4) % 2);
    digitalWrite(PC13, (i / 8) % 2);
    adc[i] = analogRead(PB0);
  }
  read_ldrs_fast();
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

  ////////////////////// distance:
  distance_r = adc[12];
  distance_b = adc[13];
  distance_l = adc[14];
  diff = (distance_r - distance_l) * 2.0;
}