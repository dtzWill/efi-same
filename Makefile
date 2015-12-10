
# Can't use clang due to builtins used in efivar.h
CC := gcc
CFLAGS := -O3 -I/usr/include/efivar
CFLAGS += -Wall -Wextra
LDFLAGS := -lefivar


TARGET := efisame

.PHONY: all clean
all: $(TARGET)

%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	-rm $(TARGET)
