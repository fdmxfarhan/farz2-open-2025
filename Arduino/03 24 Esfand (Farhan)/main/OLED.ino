void print_all() {
  display.clearDisplay();
  display.drawCircle(90, 32, 23, WHITE);
  display.drawCircle(90, -10, 25, WHITE);
  display.fillRect(30, -10, 100, 22, BLACK);
  display.drawLine(
    90 + sin(-zavie_robot * PI / 180) * 5,
    32 - cos(-zavie_robot * PI / 180) * 5,
    90 - sin(-zavie_robot * PI / 180) * 5,
    32 + cos(-zavie_robot * PI / 180) * 5,
    WHITE);
  if (ball_in_kicker) display.fillCircle(90, 10, 3, WHITE);
  else if (is_ball) display.fillCircle(90 + sin(ball_ang * PI / 180) * 29, 32 - cos(ball_ang * PI / 180) * 29, 3, WHITE);
  if (is_yellow)
    display.drawLine(
      90 + sin((yellow_angle - 10) * PI / 180) * 30,
      32 - cos((yellow_angle - 10) * PI / 180) * 30,
      90 + sin((yellow_angle + 10) * PI / 180) * 30,
      32 - cos((yellow_angle + 10) * PI / 180) * 30,
      WHITE);
  if (ldr_front) display.drawRect(89, 20, 3, 7, WHITE);
  if (ldr_back) display.drawRect(89, 45, 3, 7, WHITE);
  if (ldr_right) display.drawRect(100, 31, 8, 3, WHITE);
  if (ldr_left) display.drawRect(72, 31, 8, 3, WHITE);

  display.setCursor(0, 0);
  display.print("xb:");
  display.println(xb);
  display.print("yb:");
  display.println(yb);
  display.print("ab:");
  display.println(ball_ang);
  display.print("db:");
  display.println(ball_dist);
  display.print("R:");
  display.println(distance_r);
  display.print("B:");
  display.println(distance_b);
  display.print("L:");
  display.println(distance_l);
  display.print("GX:");
  display.println(yellow_angle);
  // display.print("period:");
  // display.println(millis() - last_time);
  // display.print("GY:");
  // display.println(zavie_robot);
  // display.print("VB:");
  // display.println(Vbat);
  display.display();
  last_time = millis();
}
void print_adc() {
  display.clearDisplay();
  for (int i = 0; i < 8; i++) {
    display.setCursor(0, i * 8);
    display.print(i);
    display.print(":");
    display.println(adc[i]);
  }
  for (int i = 8; i < 16; i++) {
    display.setCursor(64, (i - 8) * 8);
    display.print(i);
    display.print(":");
    display.println(adc[i]);
  }
  display.display();
}