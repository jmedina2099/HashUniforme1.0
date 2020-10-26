CC=g++
CARCH=$(shell $(CC) -march=native -Q --help=target | grep -- '-march='| grep -v 'option' | cut -f3)

CFLAGS=-Wextra -Wpedantic -Werror -Wall -fwrapv -Wno-vla
COPT=-O3
CDELL=-march=$(CARCH) -mtune=$(CARCH)

AZRAEL=azrael

all: $(AZRAEL) 

$(AZRAEL): $(AZRAEL).cpp
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -o $(AZRAEL) $(AZRAEL).cpp
	
clean:
	$(RM) $(AZRAEL)
	
