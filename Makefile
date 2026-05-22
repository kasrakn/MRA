CC     = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2

SRCDIR = src
TARGET = MRA
SRCS   = $(SRCDIR)/mra.c     \
         $(SRCDIR)/scanner.c  \
         $(SRCDIR)/inexact.c  \
         $(SRCDIR)/utils.c
OBJS   = $(SRCS:.c=.o)
DEPS   = $(OBJS:.o=.d)

.PHONY: all debug clean

all: $(TARGET)

debug: CFLAGS += -g -O0
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
