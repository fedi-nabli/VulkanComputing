#include <vulkan/vulkan.h>

#include "device.h"
#include "compute.h"
#include "instance.h"
#include "pipeline.h"

int main(int argc, const char** argv) 
{
  CreateInstance();
  GetPhysicalDevice();
  CreateDeviceAndComputeQueue();
  CreatePipeline();
  CreateDescriptorSet();
  CreateCommandPool();
  PrepareCommandBuffer();
  Compute();

  DestroyPipeline();
  DestroyCommandPoolAndLogicalDevice();

  return 0;
}
