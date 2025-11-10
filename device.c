#include "device.h"

#include "memory.h"
#include "instance.h"

#include <stdio.h>
#include <string.h>
#include <vulkan/vulkan.h>

uint32_t ComputeQueueFamilyIndex;

VkDevice LogicalDevice = VK_NULL_HANDLE;
VkQueue ComputingQueue = VK_NULL_HANDLE;
VkCommandPool ComputeCmdPool = VK_NULL_HANDLE;
VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;

void CreateDeviceAndComputeQueue()
{
  VkQueueFamilyProperties families[100];
  uint32_t count = 100;

  vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &count, families);

  printf("Found %u queue families\n", count);

  ComputeQueueFamilyIndex = 0;
  while ((ComputeQueueFamilyIndex < count) &&
          (families[ComputeQueueFamilyIndex].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)
  {
    ComputeQueueFamilyIndex++;
  }

  if (ComputeQueueFamilyIndex == count)
  {
    printf("Compute queue not found\n");
    return;
  }

  float prio = 1.0f;

  const char* deviceExtensions[] = {
    "VK_KHR_portability_subset"
  };

  VkDeviceQueueCreateInfo queueCreateInfo;
  memset(&queueCreateInfo, 0, sizeof(queueCreateInfo));
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = ComputeQueueFamilyIndex;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = &prio;

  VkDeviceCreateInfo deviceCreateInfo;
  memset(&deviceCreateInfo, 0, sizeof(deviceCreateInfo));
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
  deviceCreateInfo.queueCreateInfoCount = 1;
  deviceCreateInfo.enabledExtensionCount = 1;
  deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;

  if (vkCreateDevice(PhysicalDevice, &deviceCreateInfo, NULL, &LogicalDevice) != VK_SUCCESS)
  {
    printf("Failed to create logical device\n");
    return;
  }

  vkGetDeviceQueue(LogicalDevice, ComputeQueueFamilyIndex, 0, &ComputingQueue);
}

void CreateCommandPool()
{
  VkCommandPoolCreateInfo poolCreateInfo;
  memset(&poolCreateInfo, 0, sizeof(poolCreateInfo));
  poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolCreateInfo.queueFamilyIndex = ComputeQueueFamilyIndex;

  if (vkCreateCommandPool(LogicalDevice, &poolCreateInfo, NULL, &ComputeCmdPool) != VK_SUCCESS)
  {
    printf("Failed to create a command pool\n");
    return;
  }
}

void CreateDescriptorPool()
{
  VkDescriptorPoolSize descriptorPoolSize;
  memset(&descriptorPoolSize, 0, sizeof(descriptorPoolSize));
  descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  descriptorPoolSize.descriptorCount = 2;

  VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
  memset(&descriptorPoolCreateInfo, 0, sizeof(descriptorPoolCreateInfo));
  descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptorPoolCreateInfo.maxSets = 1;
  descriptorPoolCreateInfo.poolSizeCount = 1;
  descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;

  if (vkCreateDescriptorPool(LogicalDevice, &descriptorPoolCreateInfo, NULL, &DescriptorPool) != VK_SUCCESS)
  {
    printf("Failed to create the descriptor pool\n");
  }
}

void DestroyCommandPoolAndLogicalDevice()
{
  if (ComputeCmdPool != VK_NULL_HANDLE)
  {
    vkDestroyCommandPool(LogicalDevice, ComputeCmdPool, NULL);
  }

  if (DescriptorPool != VK_NULL_HANDLE)
  {
    vkDestroyDescriptorPool(LogicalDevice, DescriptorPool, NULL);
  }

  DestroyBuffers();

  if (LogicalDevice != VK_NULL_HANDLE)
  {
    vkDestroyDevice(LogicalDevice, NULL);
  }
}
