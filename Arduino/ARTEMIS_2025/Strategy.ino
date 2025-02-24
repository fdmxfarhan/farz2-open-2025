void Forward() {
  if (digitalRead(PA11)) {
    motor(0, 0, 0, 0);
  } else if (is_ball) {
    if (ball_ang < 15 || ball_ang > 345) moveAngle(0);
    else if (ball_ang < 40 || ball_ang > 320) moveAngle(ball_ang);
    else if (ball_ang < 60) moveAngle(ball_ang + 40);
    else if (ball_ang > 300) moveAngle(ball_ang - 40);
    else if (ball_ang < 180) moveAngle(ball_ang + 90);
    else moveAngle(ball_ang - 90);
  } else {
    motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
  }
}