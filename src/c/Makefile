CC=gcc
CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv
COPT=-O3
CDELL=-march=skylake -mtune=skylake
DFLAG=-DDEBUG_PARTIAL_HASH

# Comment for only calculate the hash
CPRINT=-DPRINT_HASH

TARGET = azrael

all: $(TARGET)

dbg: debug

$(TARGET): $(TARGET).c
	$(CC) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET) $(TARGET).c
	
debug: $(TARGET).c
	$(CC) $(DFLAG) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET) $(TARGET).c
     
clean:
	$(RM) $(TARGET)

	
