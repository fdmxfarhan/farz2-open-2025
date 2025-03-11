void shoot()
{
  if (shoot_cnt > 20)
  {
    shoot_cnt = 0;
    already_shooted = false;
  }
  if (already_shooted)
  {
    shoot_cnt++;
    return;
  }
  digitalWrite(PB12, 1);
  delay(70);
  digitalWrite(PB12, 0);
  shoot_cnt = 0;
  already_shooted = true;
}