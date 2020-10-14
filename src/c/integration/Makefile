CC=gcc
CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv
COPT=-O3
CDELL=-march=skylake -mtune=skylake

# Comment for only calculate the hash
CPRINT=-DPRINT_HASH

TARGET_X4 = azraelx4_link
UNIT_TEST = unit_test

all: $(TARGET_X4) $(UNIT_TEST)

dbg: debug

$(TARGET_X4): $(TARGET_X4).c
	$(CC) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_X4) $(TARGET_X4).c
	
$(UNIT_TEST): $(UNIT_TEST).c
	$(CC) $(CPRINT) $(CFLAGS) $(COPT) $(CDELL) -o $(UNIT_TEST) $(UNIT_TEST).c
	
clean:
	$(RM) $(TARGET_X4)
	$(RM) $(UNIT_TEST)	