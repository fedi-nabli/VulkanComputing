#include <vulkan/vulkan.h>

#include "device.h"
#include "compute.h"
#include "instance.h"

int main(int argc, const char** argv) 
{
  CreateInstance();
  GetPhysicalDevice();
  CreateDeviceAndComputeQueue();
  CreateCommandPool();
  PrepareCommandBuffer();
  Compute();

  DestroyCommandPoolAndLogicalDevice();

  return 0;
}
