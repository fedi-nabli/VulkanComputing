HEADERS = compute.h device.h instance.h pipeline.h
SOURCES = compute.c device.c instance.c pipeline.c main.c

TARGET = bin/vulkan_compute

SHADER_SRC = shader.comp
SHADER_BIN = bin/comp.spv

VULKAN_LIB_PATH ?= $(VULKAN_SDK)/lib
LD_FLAGS = -lvulkan -L$(VULKAN_LIB_PATH) -Wl,-rpath,$(VULKAN_LIB_PATH)

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS) $(SHADER_BIN)
	@mkdir -p bin
	gcc -g -O0 $(SOURCES) $(LD_FLAGS) -o $(TARGET)

$(SHADER_BIN): $(SHADER_SRC)
	@mkdir -p bin
	glslangValidator -V $(SHADER_SRC) -o $(SHADER_BIN)

run: $(TARGET)
	./bin/vulkan_compute

.PHONY = all clean

clean:
	rm -rf $(TARGET)
	rm -rf $(SHADER_BIN)
	rm -rf ./bin
