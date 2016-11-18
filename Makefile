
CC = gcc
OUTPUT = DelcomLinux
RM = rm

delcom_light: DelcomLinux.c
	$(CC) DelcomLinux.c -lhidapi-libusb -o $(OUTPUT)

clean:
	rm $(OUTPUT)
