# arch
#CARCH=skylake
CC=gcc
CARCH=$(shell $(CC) -march=native -Q --help=target | grep -- '-march='| grep -v 'option' | cut -f3)

CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv -Wimplicit-fallthrough=0
COPT=-O3
CDELL=-march=$(CARCH) -mtune=$(CARCH)

RUN=runit
RANDOM_DIR=../random
SHA_DIR=../sha

OUT_DIR_HASH_DEF=/spacio/azrael
OUT_DIR_HASH_XZ_DEF=/spacio/azrael

ifndef OUT_DIR_HASH
OUT_DIR_HASH=$(OUT_DIR_HASH_DEF)
endif
ifndef OUT_DIR_HASH_XZ
OUT_DIR_HASH_XZ=$(OUT_DIR_HASH_XZ_DEF)
endif

TARGET_BASE = azrael_imp_base
TARGET_64 = azrael_imp_64_link
TARGET_512 = azrael_imp_512_link
TARGET_X4 = azrael_imp_x4_link
UNIT_TEST = unit_imp_test

all: $(TARGET_BASE) $(TARGET_64) $(TARGET_512) $(TARGET_X4) $(UNIT_TEST)
	$(MAKE) -C $(RANDOM_DIR)

$(TARGET_BASE): $(TARGET_BASE).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_BASE) $(TARGET_BASE).c

$(TARGET_64): $(TARGET_64).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_64) $(TARGET_64).c

$(TARGET_512): $(TARGET_512).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_512) $(TARGET_512).c
	
$(TARGET_X4): $(TARGET_X4).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -c -o $(TARGET_X4) $(TARGET_X4).c

$(UNIT_TEST): $(UNIT_TEST).c
	$(MAKE) -C $(SHA_DIR)
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -o $(UNIT_TEST) $(UNIT_TEST).c

clean-target:
	$(RM) $(TARGET_BASE)
	$(RM) $(TARGET_64)
	$(RM) $(TARGET_512)
	$(RM) $(TARGET_X4)
	$(RM) $(UNIT_TEST)

clean-hashes:
	$(RM) $(OUT_DIR_HASH)/*.hash
	$(RM) $(OUT_DIR_HASH)/*.hash.*
	$(RM) $(OUT_DIR_HASH_XZ)/*.hash.xz
	$(RM) $(OUT_DIR_HASH_XZ)/*.hash.*.xz

clean: clean-target clean-hashes
	$(MAKE) -C $(RANDOM_DIR) clean
