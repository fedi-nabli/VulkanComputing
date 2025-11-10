#include <vulkan/vulkan.h>

#include "device.h"
#include "memory.h"
#include "compute.h"
#include "instance.h"
#include "pipeline.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h> 

uint32_t InputData[2][10000];
uint32_t OutputData[1];

void generate()
{
  uint32_t maxValue = 0;

  for (uint32_t i = 0; i < 10000; i++)
  {
    for (uint32_t j = 0; j < 10000; j++)
    {
      uint32_t value = InputData[0][i] * InputData[1][j];
      if (value > maxValue)
        maxValue = value;
    }
  }

  OutputData[0] = maxValue;
}

uint32_t get_time()
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

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

  for (uint32_t i = 0; i < 10000; i++)
  {
    InputData[0][i] = rand() % 1000;
    InputData[1][i] = rand() % 1000;
  }

  CopyToInputBuffer(InputData, sizeof(InputData));

  uint32_t time = get_time();
  generate();
  time = get_time() - time;
  printf("CPU time: %u millisecnds, value = %u\n", time, OutputData[0]);

  OutputData[0] = 0.0f;
  CopyToOutputBuffer(OutputData, sizeof(OutputData));

  time = get_time();
  Compute();
  time = get_time() - time;

  CopyFromOutputBuffer(OutputData, sizeof(OutputData));
  printf("GPU time: %u millisecnds, value = %u\n", time, OutputData[0]);

  DestroyPipeline();
  DestroyCommandPoolAndLogicalDevice();

  return 0;
}
