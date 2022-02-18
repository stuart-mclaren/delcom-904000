
CC = gcc
CFLAGS = -Wall
OUTPUT = DelcomFreeBSD
RM = rm -f

# Requires:
# hidapi-0.xx.0_x                Library to access USB HID-class devices

all: delcom_light delcom_cycle_colors delcom_red_flasher delcom_on_off

delcom_light: DelcomFreeBSD.c
	$(CC) $(CFLAGS) DelcomFreeBSD.c -lhidapi -o $@

delcom_cycle_colors: delcom_cycle_colors.c
	$(CC) $(CFLAGS) delcom_cycle_colors.c -lhidapi -o $@

delcom_red_flasher: delcom_red_flasher.c
	$(CC) $(CFLAGS) delcom_red_flasher.c -lhidapi -o $@

delcom_on_off: delcom_on_off.c
	$(CC) $(CFLAGS) delcom_on_off.c -lhidapi -o $@
clean:
	$(RM) $(OUTPUT) delcom_light delcom_cycle_colors delcom_red_flasher
