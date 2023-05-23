CC = gcc
CFLAGS = -std=c99 -Wall
LDFLAGS = -lm

SRCS = main.c
OBJS = $(SRCS:.c=.o)

TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
