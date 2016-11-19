
CC = gcc
CFLAGS = -Wall
OUTPUT = DelcomLinux
RM = rm -f

all: delcom_light delcom_cycle_colors delcom_red_flasher

delcom_light: DelcomLinux.c
	$(CC) $(CFLAGS) DelcomLinux.c -lhidapi-libusb -o $@

delcom_cycle_colors: delcom_cycle_colors.c
	$(CC) $(CFLAGS) delcom_cycle_colors.c -lhidapi-libusb -o $@

delcom_red_flasher: delcom_red_flasher.c
	$(CC) $(CFLAGS) delcom_red_flasher.c -lhidapi-libusb -o $@

clean:
	$(RM) $(OUTPUT) delcom_light delcom_cycle_colors delcom_red_flasher
