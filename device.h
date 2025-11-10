#ifndef __DEVICE_H_
#define __DEVICE_H_

#include <vulkan/vulkan.h>

extern VkDevice LogicalDevice;
extern VkQueue ComputingQueue;
extern VkCommandPool ComputeCmdPool;
extern VkDescriptorPool DescriptorPool;

void CreateCommandPool();
void CreateDescriptorPool();
void CreateDeviceAndComputeQueue();
void DestroyCommandPoolAndLogicalDevice();

#endif
