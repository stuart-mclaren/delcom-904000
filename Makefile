
CC = gcc

delcom_light: DelcomLinux.c
	$(CC) DelcomLinux.c -lhidapi-libusb -o DelcomLinux
