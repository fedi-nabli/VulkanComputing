#ifndef __DEVICE_H_
#define __DEVICE_H_

#include <vulkan/vulkan.h>

extern VkDevice LogicalDevice;
extern VkQueue ComputingQueue;
extern VkCommandPool ComputeCmdPool;

void CreateCommandPool();
void CreateDeviceAndComputeQueue();
void DestroyCommandPoolAndLogicalDevice();

#endif
