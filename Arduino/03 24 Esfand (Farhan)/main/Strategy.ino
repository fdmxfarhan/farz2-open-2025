int cnt = 0;
int out_cnt = 0;
int cnt_ball_in_kick = 0;

void Forward() {
  out();
  if (digitalRead(PA11)) {
    motor(0, 0, 0, 0);
  } else if (ball_in_kicker || ((ball_ang < 10 || ball_ang > 350) && ball_dist < 60 && is_ball)) {
    cnt_ball_in_kick++;
    V = 600;
    if (cnt_ball_in_kick > 20) {
      moveAngle(180);
      if (cnt_ball_in_kick > 40) cnt_ball_in_kick = 0;
    } else {
      moveAngle(0);
      shoot();
    }
  } else if (is_ball) {
    cnt_ball_in_kick = 0;
    cnt = 0;
    if (robot_id == 1) V = (ball_dist - 30) * 6;
    else V = (ball_dist - 30) * 6;
    if (ball_dist < 100) {
      if (ball_ang < 25 || ball_ang > 335) {
        V = 600;
        moveAngle(ball_ang);
      } else if (ball_ang < 60) moveAngle(ball_ang + 40);
      else if (ball_ang > 300) moveAngle(ball_ang - 40);
      else if (ball_ang < 180) moveAngle(ball_ang + 80);
      else moveAngle(ball_ang - 80);
    } else {
      if (ball_ang < 25 || ball_ang > 335) {
        moveAngle(ball_ang);
      } else if (ball_ang < 180) moveAngle(ball_ang + 20);
      else if (ball_ang > 180) moveAngle(ball_ang - 20);
    }
  } else {
    cnt++;
    comeback_center();
    // comeback_simple();
  }
}
void comeback_simple() {
  if (cnt < 22) {
    V = 300;
    moveAngle(180);
  } else motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
}
void comeback_center() {
  V = 200;
  if (distance_b > 150) motor(-V - diff + zavie_robot, -V + diff + zavie_robot, V + diff + zavie_robot, V - diff + zavie_robot);
  else motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
}
void reset_ldr_bools() {
  ldr_front = false;
  ldr_right = false;
  ldr_left = false;
  ldr_back = false;
}
void out() {
  int MOVE_SEC = 60;
  int OUT_TIMEOUT = 50;
  V = 600;
  if (ldr_left) {
    move4sec(90, MOVE_SEC);
    out_cnt = 0;
    reset_ldr_bools();
    while (ball_ang > 180 && is_ball && out_cnt < OUT_TIMEOUT) {
      TDread_noSkip();
      read_cam(false);
      read_sensors();
      if (digitalRead(PA11)) print_adc();
      else print_all();
      motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
      out_cnt++;
    }
    reset_ldr_bools();
  }
  if (ldr_right) {
    move4sec(270, MOVE_SEC);
    out_cnt = 0;
    reset_ldr_bools();
    while (ball_ang < 180 && is_ball && out_cnt < OUT_TIMEOUT) {
      TDread_noSkip();
      read_cam(false);
      read_sensors();
      if (digitalRead(PA11)) print_adc();
      else print_all();
      motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
      out_cnt++;
    }
    reset_ldr_bools();
  }
  if (ldr_front) {
    move4sec(180, MOVE_SEC);
    out_cnt = 0;
    reset_ldr_bools();
    while ((ball_ang < 90 || ball_ang > 270) && is_ball && out_cnt < OUT_TIMEOUT) {
      TDread_noSkip();
      read_cam(false);
      read_sensors();
      if (digitalRead(PA11)) print_adc();
      else print_all();
      motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
      out_cnt++;
    }
    reset_ldr_bools();
  }
  if (ldr_back) {
    move4sec(0, MOVE_SEC);
    out_cnt = 0;
    reset_ldr_bools();
    while (ball_ang > 90 && ball_ang < 2700 && is_ball && out_cnt < OUT_TIMEOUT) {
      TDread_noSkip();
      read_cam(false);
      read_sensors();
      if (digitalRead(PA11)) print_adc();
      else print_all();
      motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
      out_cnt++;
    }
    reset_ldr_bools();
  }
}