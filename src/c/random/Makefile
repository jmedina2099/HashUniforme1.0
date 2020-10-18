# arch
#CARCH=skylake
CC=gcc
CARCH=$(shell $(CC) -march=native -Q --help=target | grep -- '-march='| grep -v 'option' | cut -f3)

CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv
COPT=-O3
CDELL=-march=$(CARCH) -mtune=$(CARCH)

TARGET = random

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET) $(TARGET).c
	
clean:
	$(RM) $(TARGET)
