void read_cam(bool skip) {
  read_ldrs_fast();
  if(skip && (ldr_front || ldr_back || ldr_left || ldr_right)) return;
  if (robot_id == 1) {
    read_pixy();
  } else {
    read_pixy();
    // read_MV();
  }
}