// Flashes red color
// Copyright Erik Zachrisson: erik@zachrisson.info
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

#include "delcom_freebsd.h"

static HIDPacketStruct my_packet;

int write_hid(hid_device *hnd, uint8_t data) {
  // Write 8 byte command
  my_packet.tx.major_cmd = WRITE_PORT_CMD;
  // Write to port 1 command
  my_packet.tx.minor_cmd = 2;
  // Data to write to port1
  my_packet.tx.data_lsb = data;
  // Send it
  return hid_send_feature_report(hnd, my_packet.data, 8);
}

void usage() {
    printf("Usage:\n");
    printf("Turn off light\n");
    printf("./delcom_on_off off\n");
    printf("Turn on red light\n");
    printf("./delcom_on_off red\n");
    printf("Turn on green light\n");
    printf("./delcom_on_off green\n");
    printf("Turn on yellow light\n");
    printf("./delcom_on_off yellow\n");
    return;
}

int main(int argc, char* argv[]) {
  hid_device *handle;
  uint32_t colour;

  if (argc != 2){
    usage();
    return errno;
  }
 
  if (strcmp(argv[1],"red") == 0){
    printf("red\n");
    colour = RED_COLOR;
  } else if (strcmp(argv[1],"green") == 0){
    printf("green\n");
    colour = GREEN_COLOR;
  } else if (strcmp(argv[1],"yellow") == 0){
    printf("yello\n");
    colour = YELLOW_COLOR;
  } else if (strcmp(argv[1],"off") == 0){
    printf("off\n");
    colour = NO_COLOR;
  } else {
    printf("unknown colour: %s\n", argv[1]);
    usage();
    return errno;
  }

  // Open the device using the VID, PID,
  handle = hid_open(DELCOM_HID_VID, DELCOM_HID_PID, NULL);
  if (!handle) {
    printf("Error: Unable to open device. (%d:%s)\n", errno, strerror(errno));
    return errno;
  }

  write_hid(handle, colour);

  return 0;
}
