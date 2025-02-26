void Forward() {
  if (digitalRead(PA11)) {
    motor(0, 0, 0, 0);
  } else if (ball_in_kicker){
    V = 600;
    moveAngle(0);
    shoot();
  } else if (is_ball) {
    V = ball_dist * 5;
    if(ball_dist < 80){
      if (ball_ang < 20 || ball_ang > 340) moveAngle(ball_ang);
      else if (ball_ang < 60) moveAngle(ball_ang + 40);
      else if (ball_ang > 300) moveAngle(ball_ang - 40);
      else if (ball_ang < 180) moveAngle(ball_ang + 70);
      else moveAngle(ball_ang - 70);
    }else moveAngle(ball_ang);
  } else {
    motor(zavie_robot, zavie_robot, zavie_robot, zavie_robot);
  }
}