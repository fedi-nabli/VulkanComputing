#include <vulkan/vulkan.h>

#include "device.h"
#include "memory.h"
#include "compute.h"
#include "instance.h"
#include "pipeline.h"

#include <stdio.h>

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

  for (uint32_t i = 0; i < 1000; i++)
  {
    InputData[i] = i;
    OutputData[i] = 0.0f;
  }

  CopyToInputBuffer(InputData, sizeof(InputData));
  Compute();
  CopyFromOutputBuffer(OutputData, sizeof(OutputData));

  for (uint32_t i = 0; i < 1000; i += 50)
  {
    printf("%u -> %f\n", InputData[i], OutputData[i]);
  }

  DestroyPipeline();
  DestroyCommandPoolAndLogicalDevice();

  return 0;
}
