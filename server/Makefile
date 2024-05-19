CC := gcc
CFLAGS := -Wall -Wextra 


.PHONY: all
all: aesdsocket

# Cross-compilation support
ifeq ($(filter aarch64-none-linux-gnu-%,$(CROSS_COMPILE)),aarch64-none-linux-gnu-)
    CC := $(CROSS_COMPILE)$(CC)
endif

# Source files
SRC := aesdsocket.c

# Object files
OBJ := $(SRC:.c=.o)

# Build the writer application
writer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Clean target: remove writer application and object files
.PHONY: clean
clean:
	rm -f aesdsocket $(OBJ)
