int get_angle(int x, int y)
{
  int angle = atan2(x - xr, y - yr) * 180 / PI;
  if (angle < 0)
    angle += 360;
  return angle;
}
void read_MV() {
  Wire.requestFrom(openMV_Address, 12);  // Request 12 bytes (6 values * 2 bytes each)
  if (Wire.available() == 12) {
    uint8_t buffer[12];
    for (int i = 0; i < 12; i++) {
      buffer[i] = Wire.read();  // Read each byte into buffer
    }

    // Unpack the data
    xb = (int16_t)(buffer[0] | (buffer[1] << 8));
    yb = (int16_t)(buffer[2] | (buffer[3] << 8));
    yellow_x = (int16_t)(buffer[4] | (buffer[5] << 8));
    yellow_y = (int16_t)(buffer[6] | (buffer[7] << 8));
    blue_x = (int16_t)(buffer[8] | (buffer[9] << 8));
    blue_y = (int16_t)(buffer[10] | (buffer[11] << 8));

    ball_ang = get_angle(xb, yb);
    ball_dist = sqrt(pow(xb - xr, 2) + pow(yb - yr, 2));
    yellow_angle = get_angle(yellow_x, yellow_y);
    distance_yellow = sqrt(pow(yellow_x - xr, 2) + pow(yellow_y - yr, 2));
    blue_angle = get_angle(blue_x, blue_y);
    distance_blue = sqrt(pow(blue_x - xr, 2) + pow(blue_y - yr, 2));
    if (xb == 0 && yb == 0)
      is_ball = false;
    else
      is_ball = true;
    if (yellow_x == 0 && yellow_y == 0)
      is_yellow = false;
    else
      is_yellow = true;
    if (blue_x == 0 && blue_y == 0)
      is_blue = false;
    else
      is_blue = true;
  }
}