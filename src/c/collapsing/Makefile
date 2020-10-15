CC=gcc
//CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv
CFLAGS=-pthread
COPT=-O3
CDELL=-march=skylake -mtune=skylake

TARGET_AVA = azrael-avalanche

all: $(TARGET_AVA)

$(TARGET_AVA): $(TARGET_AVA).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET_AVA) $(TARGET_AVA).c

clean:
	$(RM) $(TARGET_AVA)
