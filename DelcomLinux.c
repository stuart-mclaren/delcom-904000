// Delcom Products Inc - 200 William St STE302 Port Chester NY 10573 USA // DelcomLinux - A simple linux example to read and write the Delcom USB Device.
// Works with all Delcom HID device (Generations 2,3+).
// Version 1.0 April 8, 015 - Tested on Ubuntu V14 // For Delcom Device commands see http://www.delcomproducts.com/downloads/USBIOHID.pdf
// Requires Lib: hidapi - You must install this package - // http://packages.ubuntu.com/source/trusty/hidapi
// Compile String:  gcc -g -O0 -o tryme DelcomLinux.c -lhidapi-libusb // To run: './tryme'  (or 'sudo ./tryme' if you need other rights)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hidapi/hidapi.h>

typedef union  HIDPacketStruct {
     unsigned char  Data[256];
     struct {
         unsigned char MajorCmd;
         unsigned char MinorCmd;
         unsigned char DataLSB;
         unsigned char DataMSB;
         unsigned char DataHID[4];
         unsigned char DataExt[8];
              } Tx;
      struct {
                unsigned char Cmd;
         } Rx;
        } HIDPacketStruct, *pHIDPacketStruct;




int main(int argc, char* argv[])
{
     char Port0, Port1;
     int res, count, cmd;
     hid_device *handle;
     int report_id;
     HIDPacketStruct MyPacket;

     printf("\n");
     cmd = -1;
         if (argc > 1) {              // Check for command line arguments
         if(isdigit(argv[1][0]) )
             cmd = (int) strtol(argv[1], NULL, 0);
         else      {
             printf("Delcom USB Linux Example. Version 1.0.\n");
             printf("Syntax: tryme [cmd]\n");
             printf("With no arguments, just reads the ports.\n");
             printf("With numeric argument, XORs the value with port1 and write it to port1.\n");
             printf("For example 'tryme 1' will toggle bit 0 on port 1.\n");
             return 1;
         }
     }



     // Open the device using the VID, PID,
     handle = hid_open(0xfc5, 0xb080, NULL);
     if (!handle) {
         printf("Error: Unable to open device.\n");
          return 1;
     }

     // device found, read device info and display
     printf("Delcom Device found. ");
     MyPacket.Rx.Cmd = 10;        // Read Version (Command #10)
     res = hid_get_feature_report(handle, MyPacket.Data, 8);
     if (res < 0) {
         printf("Error: Failed to read device.\n");
         printf("%ls", hid_error(handle));
         return 1;
     }
     else     printf("Firmware Version: %d\n", MyPacket.Data[4]);


     // Read the ports (Command #100).
     MyPacket.Rx.Cmd = 100;
     res = hid_get_feature_report(handle, MyPacket.Data, 8);
     if (res < 0) {
         printf("Error: Failed to read device.\n");
         printf("%ls", hid_error(handle));
         return 1;
     }
     else {     // Get and Display the current pin values
         Port0 = MyPacket.Data[0];
         Port1 = MyPacket.Data[1];
         printf("Port0: 0x%02hhx Port1: 0x%02hhx\n", Port0, Port1);
     }


     // Now do the write port1 command (if cmd!=-1)
     if(cmd!=-1){
         Port1 ^= (char)cmd;        // XOR the port1 value
         printf("Writing value 0x%02hhx to port1.\n",Port1);
         MyPacket.Tx.MajorCmd = 101;    // Write 8 byte command
         MyPacket.Tx.MinorCmd = 2;    // Write to port 1 command
         MyPacket.Tx.DataLSB = Port1;    // Data to write to Port1
         hid_send_feature_report(handle, MyPacket.Data, 8);    // Send it
         }



     printf("All done, closing device and HIDAPI objects!\n\n");
     hid_close(handle);
     hid_exit();

     return 0;
}
