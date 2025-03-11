int cnt = 0;
void Forward() {
  // if (ldr_left) {
  //   motor(0, 0, 0, 0);
  //   motor(0, 0, 0, 0);
  //   V = 600;
  //   move4sec(90, 2);
  //   while (ball_ang > 200 && ball_ang < 340) {
  //     TDread();
  //     read_pixy();
  //     read_ldr();
  //     if (digitalRead(PA11)) print_adc();
  //     else print_all();
  //     motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
  //   }
  // }
  // if (ldr_right) {
  //   motor(0, 0, 0, 0);
  //   motor(0, 0, 0, 0);
  //   V = 600;
  //   move4sec(270, 2);
  //   while (ball_ang < 150 && ball_ang > 10) {
  //     TDread();
  //     read_pixy();
  //     read_ldr();
  //     if (digitalRead(PA11)) print_adc();
  //     else print_all();
  //     motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
  //   }
  // }
  // if (ldr_front) {

  //   motor(0, 0, 0, 0);
  //   motor(0, 0, 0, 0);

  //   V = 600;
  //   move4sec(180, 2);
  //   while (ball_ang < 20 && ball_ang > 340) {
  //     TDread();
  //     read_pixy();
  //     read_ldr();
  //     if (digitalRead(PA11)) print_adc();
  //     else print_all();
  //     motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
  //   }
  // }

  if (digitalRead(PA11)) {
    motor(0, 0, 0, 0);
  } else if (ball_in_kicker) {
    V = 600;
    moveAngle(0);
    shoot();
  } else if (is_ball) {
    cnt = 0;
    if (robot_id == 1) V = (ball_dist - 30) * 8;
    else V = (ball_dist - 30) * 5;
    if (ball_dist < 100) {
      if (ball_ang < 25 || ball_ang > 335) {
        V = 600;
        moveAngle(ball_ang);
      } else if (ball_ang < 60) moveAngle(ball_ang + 50);
      else if (ball_ang > 300) moveAngle(ball_ang - 50);
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
    if (cnt < 18) {
      V = 200;
      moveAngle(180);
    } else motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
  }
}