
CC = gcc
CFLAGS = -Wall
OUTPUT = DelcomLinux
RM = rm

delcom_light: DelcomLinux.c
	$(CC) $(CFLAGS) DelcomLinux.c -lhidapi-libusb -o $(OUTPUT)

clean:
	rm $(OUTPUT)
