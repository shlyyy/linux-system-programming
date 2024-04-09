CC := gcc
CFLAGS := -Wall -g

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,build/%.o,$(SRCS))
TARGETS := $(patsubst %.c,build/%,$(SRCS))
BUILD_DIR := build
INCLUDE_DIR := ../Include

.PHONY: all clean rebuild

all: $(TARGETS)

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(BUILD_DIR)

rebuild: clean all
