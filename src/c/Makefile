CC=gcc
CFLAGS=-Wall -fwrapv
COPT=-O3
CDELL=-march=skylake -mtune=skylake

TARGET = azrael

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(COPT) $(CDELL) -o $(TARGET) $(TARGET).c
     
clean:
	$(RM) $(TARGET)

	
