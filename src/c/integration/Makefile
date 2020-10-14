CC=gcc
CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv
COPT=-O3
CDELL=-march=skylake -mtune=skylake

TARGET_64 = azrael64_link
TARGET_512 = azrael512_link
TARGET_X4 = azraelx4_link
UNIT_TEST = unit_test

all: $(TARGET_64) $(TARGET_512) $(TARGET_X4) $(UNIT_TEST)

$(TARGET_64): $(TARGET_64).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_64) $(TARGET_64).c

$(TARGET_512): $(TARGET_512).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_512) $(TARGET_512).c

$(TARGET_X4): $(TARGET_X4).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_X4) $(TARGET_X4).c
	
$(UNIT_TEST): $(UNIT_TEST).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -o $(UNIT_TEST) $(UNIT_TEST).c
	
clean:
	$(RM) $(TARGET_64)
	$(RM) $(TARGET_512)
	$(RM) $(TARGET_X4)
	$(RM) $(UNIT_TEST)	