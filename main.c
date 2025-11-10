#include <vulkan/vulkan.h>

#include "device.h"
#include "memory.h"
#include "compute.h"
#include "instance.h"
#include "pipeline.h"

uint32_t InputData[1000];
float OutputData[1000];

int main(int argc, const char** argv) 
{
  CreateInstance();
  GetPhysicalDevice();
  CreateDeviceAndComputeQueue();
  CreatePipeline();
  CreateDescriptorSet();
  CreateBuffer(sizeof(InputData), sizeof(OutputData));
  CreateCommandPool();
  PrepareCommandBuffer();
  Compute();

  DestroyPipeline();
  DestroyCommandPoolAndLogicalDevice();

  return 0;
}
