# arch
#CARCH=skylake
CC=gcc
CARCH=$(shell $(CC) -march=native -Q --help=target | grep -- '-march='| grep -v 'option' | cut -f3)

CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv
COPT=-O3
CDELL=-march=$(CARCH) -mtune=$(CARCH)

RUN=runit
RANDOM_DIR=../random

OUT_DIR_HASH_DEF=/spacio/azrael
OUT_DIR_HASH_XZ_DEF=/spacio/azrael

ifndef OUT_DIR_HASH
OUT_DIR_HASH=$(OUT_DIR_HASH_DEF)
endif
ifndef OUT_DIR_HASH_XZ
OUT_DIR_HASH_XZ=$(OUT_DIR_HASH_XZ_DEF)
endif

TARGET_BASE = azrael_base
TARGET_64 = azrael64_link
TARGET_128 = azrael128_link
TARGET_192 = azrael192_link
TARGET_320 = azrael320_link
TARGET_512 = azrael512_link
TARGET_X4 = azraelx4_link
TARGET_XM = azraelxM_link
TARGET_XMM = azraelxMM_link
UNIT_TEST = unit_test

all: $(TARGET_BASE) $(TARGET_64) $(TARGET_128) $(TARGET_192) $(TARGET_320) $(TARGET_512) \
	$(TARGET_X4) $(TARGET_XM) $(TARGET_XMM) $(UNIT_TEST)
	$(MAKE) -C $(RANDOM_DIR)

$(TARGET_BASE): $(TARGET_BASE).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_BASE) $(TARGET_BASE).c

$(TARGET_64): $(TARGET_64).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_64) $(TARGET_64).c

$(TARGET_128): $(TARGET_128).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_128) $(TARGET_128).c

$(TARGET_192): $(TARGET_192).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_192) $(TARGET_192).c

$(TARGET_320): $(TARGET_320).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_320) $(TARGET_320).c

$(TARGET_512): $(TARGET_512).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_512) $(TARGET_512).c

$(TARGET_X4): $(TARGET_X4).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_X4) $(TARGET_X4).c
	
$(TARGET_XM): $(TARGET_XM).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_XM) $(TARGET_XM).c
	
$(TARGET_XMM): $(TARGET_XMM).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_XMM) $(TARGET_XMM).c
	
$(UNIT_TEST): $(UNIT_TEST).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -o $(UNIT_TEST) $(UNIT_TEST).c

run:
	./$(RUN)

test: all
	./$(UNIT_TEST)
	
clean-target:
	$(RM) $(TARGET_BASE)
	$(RM) $(TARGET_64)
	$(RM) $(TARGET_128)
	$(RM) $(TARGET_192)
	$(RM) $(TARGET_320)
	$(RM) $(TARGET_512)
	$(RM) $(TARGET_X4)
	$(RM) $(TARGET_XM)
	$(RM) $(TARGET_XMM)
	$(RM) $(UNIT_TEST)	
	
clean-hashes:
	$(RM) $(OUT_DIR_HASH)/*.hash
	$(RM) $(OUT_DIR_HASH)/*.hash.*
	$(RM) $(OUT_DIR_HASH_XZ)/*.hash.xz
	$(RM) $(OUT_DIR_HASH_XZ)/*.hash.*.xz

clean: clean-target clean-hashes
	$(MAKE) -C $(RANDOM_DIR) clean
