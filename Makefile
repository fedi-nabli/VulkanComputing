HEADERS = compute.h device.h instance.h
SOURCES = compute.c device.c instance.c main.c

TARGET = bin/vulkan_compute

VULKAN_LIB_PATH ?= $(VULKAN_SDK)/lib
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
	rm -rf ./bin
