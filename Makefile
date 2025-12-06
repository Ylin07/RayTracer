TARGET = RayTracer

SRC_DIR     = src
INCLUDE_DIR = include
EXTERNAL_DIR = external
BUILD_DIR   = build

CC      = g++
CFLAGS  = -Wall -Wextra -O3 -std=c++11
# 如果你想调试，可以改为：-O0 -g
# CFLAGS = -Wall -Wextra -O0 -g -std=c++11

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(EXTERNAL_DIR) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

output.ppm: $(TARGET)
	./$(TARGET) > output.ppm

clean:
	rm -rf $(BUILD_DIR) output.ppm $(TARGET)

rebuild: clean all

run: output.ppm
	feh -F --zoom 200 output.ppm

debug: CFLAGS = -O0 -g
debug: rebuild

.PHONY: all clean rebuild $(BUILD_DIR) run debug