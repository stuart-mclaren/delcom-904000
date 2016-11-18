// Delcom Products Inc - 200 William St STE302 Port Chester NY 10573 USA
// DelcomLinux - A simple linux example to read and write the Delcom USB Device.
// Works with all Delcom HID device (Generations 2,3+).
// Version 1.0 April 8, 015 - Tested on Ubuntu V14
// For Delcom Device commands see http://www.delcomproducts.com/downloads/USBIOHID.pdf
// Requires Lib: hidapi - You must install this package -
// http://packages.ubuntu.com/source/trusty/hidapi
// Compile String:  gcc -g -O0 -o tryme DelcomLinux.c -lhidapi-libusb
// To run: './tryme'  (or 'sudo ./tryme' if you need other rights)
// Modifications done by Erik Zachrisson - erik@zachrisson.info - 2016/11/18

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <hidapi/hidapi.h>
#include <errno.h>

typedef union HIDPacketStruct {
  uint8_t Data[256];
  struct {
    uint8_t MajorCmd;
    uint8_t MinorCmd;
    uint8_t DataLSB;
    uint8_t DataMSB;
    uint8_t DataHID[4];
    uint8_t DataExt[8];
  } Tx;
  struct {
    uint8_t Cmd;
  } Rx;
} HIDPacketStruct, *pHIDPacketStruct;

int main(int argc, char* argv[]) {
  char Port0, Port1;
  int res, count, cmd;
  hid_device *handle;
  int report_id;
  HIDPacketStruct MyPacket;

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
  handle = hid_open(0x0fc5, 0xb080, NULL);
  if (!handle) {
    printf("Error: Unable to open device. (%d:%s)\n", errno, strerror(errno));
    return errno;
  }

  // device found, read device info and display
  printf("Delcom Device found.\n");
  // Read Version (Command #10)
  MyPacket.Rx.Cmd = 10;
  res = hid_get_feature_report(handle, MyPacket.Data, 8);
  if (res < 0) {
    printf("Error: Failed to read device (%d:%s)\n", errno, strerror(errno));
    printf("%ls", hid_error(handle));
    return errno;
  } else {
    printf("Firmware Version: %d\n", MyPacket.Data[4]);
  }

  // Read the ports (Command #100).
  MyPacket.Rx.Cmd = 100;
  res = hid_get_feature_report(handle, MyPacket.Data, 8);
  if (res < 0) {
    printf("Error: Failed to read device (%d:%s).\n", errno, strerror(errno));
    printf("%ls", hid_error(handle));
    return errno;
  }
  else {     // Get and Display the current pin values
    Port0 = MyPacket.Data[0];
    Port1 = MyPacket.Data[1];
    printf("Port0: 0x%02hhx Port1: 0x%02hhx\n", Port0, Port1);
  }

  // Now do the write port1 command (if cmd!=-1)
  if (cmd != -1) {
    // XOR the port1 value
    Port1 ^= (char) cmd;
    printf("Writing value 0x%02hhx to port1.\n", Port1);
    // Write 8 byte command
    MyPacket.Tx.MajorCmd = 101;
    // Write to port 1 command
    MyPacket.Tx.MinorCmd = 2;
    // Data to write to Port1
    MyPacket.Tx.DataLSB = Port1;
    // Send it
    hid_send_feature_report(handle, MyPacket.Data, 8);
  }

  printf("All done, closing device and HIDAPI objects!\n");
  hid_close(handle);
  hid_exit();

  return 0;
}
