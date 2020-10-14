CC=gcc
CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv
COPT=-O3
CDELL=-march=skylake -mtune=skylake

TARGET_512 = azrael512_link
TARGET_X4 = azraelx4_link
UNIT_TEST = unit_test

all: $(TARGET_512) $(TARGET_X4) $(UNIT_TEST)

$(TARGET_512): $(TARGET_512).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_512) $(TARGET_512).c

$(TARGET_X4): $(TARGET_X4).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_X4) $(TARGET_X4).c
	
$(UNIT_TEST): $(UNIT_TEST).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -o $(UNIT_TEST) $(UNIT_TEST).c
	
clean:
	$(RM) $(TARGET_512)
	$(RM) $(TARGET_X4)
	$(RM) $(UNIT_TEST)	