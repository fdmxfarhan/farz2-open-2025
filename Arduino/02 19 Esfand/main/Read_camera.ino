void read_cam() {
  if (robot_id == 1) {
    read_pixy();
  } else {
    read_MV();
  }
}