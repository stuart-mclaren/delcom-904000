
#ifndef _DELCOM_LINUX_H_
#define _DELCOM_LINUX_H_

#include <stdint.h>
#include <hidapi/hidapi.h>

#define DELCOM_HID_VID 0x0FC5
#define DELCOM_HID_PID 0xB080

#define READ_VERSION_CMD  10
#define READ_PORT_CMD    100
#define WRITE_PORT_CMD   101

typedef union HIDPacketStruct {
  uint8_t data[256];
  struct {
    uint8_t major_cmd;
    uint8_t minor_cmd;
    uint8_t data_lsb;
    uint8_t data_msb;
    uint8_t data_hid[4];
    uint8_t data_ext[8];
  } tx;
  struct {
    uint8_t cmd;
  } rx;
} HIDPacketStruct, *pHIDPacketStruct;

#endif
