uint16_t blocks;

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
  if (robot_id == 1) {
    if (adc[2] > 700 || adc[1] > 750) ldr_right = true;
    else ldr_right = false;
    if (adc[4] > 970 || adc[5] > 970) ldr_front = true;
    else ldr_front = false;
    // if (adc[3] > 900 || adc[4] > 900) ldr_back = true;
    // else ldr_back = false;
    if (adc[7] > 800 || adc[6] > 800) ldr_left = true;
    else ldr_left = false;
  } else {
    if (adc[1] > 750) ldr_right = true;
    else ldr_right = false;
    if (adc[4] > 970 || adc[5] > 970) ldr_front = true;
    else ldr_front = false;
    if (adc[3] > 900 || adc[4] > 900) ldr_back = true;
    else ldr_back = false;
    if (adc[6] > 790) ldr_left = true;
    else ldr_left = false;
  }

  /////// Battery Voltage Alarm
  Vbat = adc[13] / 18.0 - 9.7;
  if (Vbat < 9 && Vbat > 5) {
    if (millis() - bat_alarm_time < 200) {
      digitalWrite(PB9, 1);
    } else if (millis() - bat_alarm_time < 2000) {
      digitalWrite(PB9, 0);
    } else bat_alarm_time = millis();
  } else digitalWrite(PB9, 0);

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
}