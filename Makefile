HEADERS = compute.h
SOURCES = compute.c main.c

TARGET = bin/vulkan_compute

VULKAN_LIB_PATH ?= $(VULKAN_SDK)/macos/lib
LD_FLAGS = -lvulkan -L$(VULKAN_LIB_PATH) -Wl,-rpath,$(VULKAN_LIB_PATH)

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	@mkdir -p bin
	gcc -g -O0 $(SOURCES) $(LD_FLAGS) -o $(TARGET)

run: $(TARGET)
	./bin/vulkan_compute

.PHONY = all clean

clean:
	rm -rf $(TARGET)
