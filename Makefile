CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm

SRC = lsb.c
OUT = lsb

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

clean:
	rm -f $(OUT)

