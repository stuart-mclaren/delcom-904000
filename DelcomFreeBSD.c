// Delcom Products Inc - 200 William St STE302 Port Chester NY 10573 USA
// DelcomFreeBSD - A simple freebsd example to read and write the Delcom USB Device.
// Works with all Delcom HID device (Generations 2,3+).
// Version 1.0 April 8, 015 - Tested on Ubuntu V14
// For Delcom Device commands see http://www.delcomproducts.com/downloads/USBIOHID.pdf
// Requires Lib: hidapi - You must install this package -
// http://packages.ubuntu.com/source/trusty/hidapi
// Compile String:  gcc -g -O0 -o tryme DelcomLinux.c -lhidapi-libusb
// To run: './tryme' (or 'sudo ./tryme' if you need other rights)
// Modifications done by Erik Zachrisson - erik@zachrisson.info - 2016/11/18

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "delcom_freebsd.h"

int main(int argc, char* argv[]) {
  uint8_t port0, port1;
  int res, cmd;
  hid_device *handle;
  HIDPacketStruct my_packet;

  cmd = -1;
  // Check for command line arguments
  if (argc > 1) {
    if (isdigit(argv[1][0])) {
      cmd = (int) strtol(argv[1], NULL, 0);
    } else {
      printf("Delcom USB Linux Example. Version 1.0.\n");
      printf("Syntax: tryme [cmd]\n");
      printf("With no arguments, just reads the ports.\n");
      printf("With numeric argument, XORs the value with port1 and write it to port1.\n");
      printf("For example 'tryme 1' will toggle bit 0 on port 1.\n");
      return 1;
    }
  }

  // Open the device using the VID, PID,
  handle = hid_open(DELCOM_HID_VID, DELCOM_HID_PID, NULL);
  if (!handle) {
    printf("Error: Unable to open device. (%d:%s)\n", errno, strerror(errno));
    return errno;
  }

  my_packet.rx.cmd = READ_VERSION_CMD;
  res = hid_get_feature_report(handle, my_packet.data, 8);
  if (res < 0) {
    printf("Error: Failed to read device (%d:%s)\n", errno, strerror(errno));
    printf("%ls", hid_error(handle));
    return errno;
  } else {
    printf("Firmware version: %d\n", my_packet.data[4]);
  }

  my_packet.rx.cmd = READ_PORT_CMD;
  res = hid_get_feature_report(handle, my_packet.data, 8);
  if (res < 0) {
    printf("Error: Failed to read device (%d:%s).\n", errno, strerror(errno));
    printf("%ls", hid_error(handle));
    return errno;
  } else {
    // Get and Display the current pin values
    port0 = my_packet.data[0];
    port1 = my_packet.data[1];
    printf("port0: 0x%02hhx, port1: 0x%02hhx\n", port0, port1);
  }

  // Now do the write port1 command (if cmd!=-1)
  if (cmd != -1) {
    // XOR the port1 value
    port1 ^= (char) cmd;
    printf("Writing value 0x%02hhx to port1.\n", port1);
    // Write 8 byte command
    my_packet.tx.major_cmd = WRITE_PORT_CMD;
    // Write to port 1 command
    my_packet.tx.minor_cmd = 2;
    // Data to write to port1
    my_packet.tx.data_lsb = port1;
    // Send it
    hid_send_feature_report(handle, my_packet.data, 8);
  }

  printf("All done, closing device and HIDAPI objects!\n");
  hid_close(handle);
  hid_exit();

  return 0;
}
