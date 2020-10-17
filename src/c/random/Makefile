CC=gcc
CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv
COPT=-O3
CDELL=-march=skylake -mtune=skylake

TARGET = random

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET) $(TARGET).c
	
clean:
	$(RM) $(TARGET)
