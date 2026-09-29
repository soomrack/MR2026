CC = gcc
CFLAGS = -Wall -g -Werror=attributes -MMD -MP

BUILD_DIR = build
NAME = Simulator

EXCLUDED_FILE = ./events/example_event/example_event.c

SRCS := $(shell find . -name '*.c' -not -path './$(BUILD_DIR)/*')
SRCS := $(filter-out $(EXCLUDED_FILE),$(SRCS))
OBJS := $(patsubst ./%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

program: $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(NAME) $(NAME).exe

.PHONY: program clean

-include $(DEPS)
