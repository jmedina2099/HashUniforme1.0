CC=gcc
CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv
COPT=-O3
CDELL=-march=skylake -mtune=skylake
DFLAG=-DDEBUG_PARTIAL_HASH

# Comment for only calculate the hash
CPRINT=-DPRINT_HASH

TARGET = azrael
TARGET_64 = azrael64
TARGET_X4 = azraelx4

all: $(TARGET) $(TARGET_64) $(TARGET_X4)

dbg: debug

$(TARGET): $(TARGET).c
	$(CC) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET) $(TARGET).c

$(TARGET_64): $(TARGET_64).c
	$(CC) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET_64) $(TARGET_64).c
	
$(TARGET_X4): $(TARGET_X4).c
	$(CC) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET_X4) $(TARGET_X4).c
	
debug: $(TARGET).c $(TARGET_64).c $(TARGET_X4).c
	$(CC) $(DFLAG) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET) $(TARGET).c
	$(CC) $(DFLAG) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET_64) $(TARGET_64).c
	$(CC) $(DFLAG) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET_X4) $(TARGET_X4).c
     
clean:
	$(RM) $(TARGET)
	$(RM) $(TARGET_64)
	$(RM) $(TARGET_X4)

	
