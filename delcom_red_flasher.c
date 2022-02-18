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

int main(int argc, char* argv[]) {
  hid_device *handle;

  // Open the device using the VID, PID,
  handle = hid_open(DELCOM_HID_VID, DELCOM_HID_PID, NULL);
  if (!handle) {
    printf("Error: Unable to open device. (%d:%s)\n", errno, strerror(errno));
    return errno;
  }

  while (true) {
    write_hid(handle, RED_COLOR);
    usleep(1000000);
    write_hid(handle, NO_COLOR);
    usleep(1000000);
  }
  return 0;
}
