#include "memory.h"

#include "device.h"
#include "compute.h"
#include "instance.h"

#include <stdio.h>
#include <string.h>
#include <vulkan/vulkan.h>

VkBuffer InputBuffer = VK_NULL_HANDLE;
VkBuffer OutputBuffer = VK_NULL_HANDLE;
VkDeviceMemory InputBufferMemory = VK_NULL_HANDLE;
VkDeviceMemory OutputBufferMemory = VK_NULL_HANDLE;

uint32_t FindMemoryIndexByType(uint32_t allowedTypesMask, VkMemoryPropertyFlags flags)
{
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &memProperties);

  uint32_t typeMask = 1;
  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++, typeMask <<= 1)
  {
    if ((allowedTypesMask & typeMask) != 0)
    {
      if ((memProperties.memoryTypes[i].propertyFlags & flags) == flags)
      {
        // Index of memory type we are looking for
        return i;
      }
    }
  }

  printf("Failed to find memory type index\n");
  return 0;
}

VkBuffer CreateBufferAndMemory(uint32_t size, VkDeviceMemory* deviceMemory, VkBufferUsageFlags usage, VkMemoryPropertyFlagBits memProperties)
{
  VkBuffer buffer;
  VkDeviceMemory memory;

  VkBufferCreateInfo bufferInfo;
  memset(&bufferInfo, 0, sizeof(bufferInfo));
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  bufferInfo.usage = usage;

  if (vkCreateBuffer(LogicalDevice, &bufferInfo, NULL, &buffer) != VK_SUCCESS)
  {
    printf("Failed to create a buffer\n");
    return VK_NULL_HANDLE;
  }

  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(LogicalDevice, buffer, &memoryRequirements);

  VkMemoryAllocateInfo memAllocInfo;
  memset(&memAllocInfo, 0, sizeof(memAllocInfo));
  memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  memAllocInfo.allocationSize = memoryRequirements.size;
  memAllocInfo.memoryTypeIndex = FindMemoryIndexByType(memoryRequirements.memoryTypeBits, memProperties);

  if (vkAllocateMemory(LogicalDevice, &memAllocInfo, NULL, &memory) != VK_SUCCESS)
  {
    printf("Failed to allocate memory for the buffer\n");
    vkDestroyBuffer(LogicalDevice, buffer, NULL);
    return VK_NULL_HANDLE;
  }

  if (vkBindBufferMemory(LogicalDevice, buffer, memory, 0) != VK_SUCCESS)
  {
    printf("Failed to bind buffer and memory\n");
  }

  *deviceMemory = memory;
  return buffer;
}

void CreateBuffer(uint32_t inputSize, uint32_t outputSize)
{
  InputBuffer = CreateBufferAndMemory(inputSize, &InputBufferMemory, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  OutputBuffer = CreateBufferAndMemory(outputSize, &OutputBufferMemory, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

  VkDescriptorBufferInfo descriptorBuffers[2];
  descriptorBuffers[0].buffer = InputBuffer;
  descriptorBuffers[0].offset = 0;
  descriptorBuffers[0].range = inputSize;
  descriptorBuffers[1].buffer = OutputBuffer;
  descriptorBuffers[1].offset = 0;
  descriptorBuffers[1].range = outputSize;

  VkWriteDescriptorSet writeDescriptorSet;
  memset(&writeDescriptorSet, 0, sizeof(writeDescriptorSet));
  writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  writeDescriptorSet.dstSet = DescriptorSet;
  writeDescriptorSet.dstBinding = 0;
  writeDescriptorSet.descriptorCount = 2;
  writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  writeDescriptorSet.pBufferInfo = descriptorBuffers;

  vkUpdateDescriptorSets(LogicalDevice, 1, &writeDescriptorSet, 0, NULL);
}

void DestroyBuffers()
{
  vkDestroyBuffer(LogicalDevice, InputBuffer, NULL);
  vkFreeMemory(LogicalDevice, InputBufferMemory, NULL);
  vkDestroyBuffer(LogicalDevice, OutputBuffer, NULL);
  vkFreeMemory(LogicalDevice, OutputBufferMemory, NULL);
}

void CopyData(void* data, uint32_t size, int isWrite, VkDeviceMemory memory)
{
  void* address;

  if (vkMapMemory(LogicalDevice, memory, 0, size, 0, &address) != VK_SUCCESS)
  {
    printf("Failed to map buffer memory\n");
    return;
  }

  if (isWrite)
  {
    memcpy(address, data, size);
  }
  else
  {
    memcpy(data, address, size);
  }

  vkUnmapMemory(LogicalDevice, memory);
}

void CopyBufferToBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t size)
{
  VkCommandBuffer cmdBuffer;

  VkCommandBufferAllocateInfo cmdBufAllocInfo;
  memset(&cmdBufAllocInfo, 0, sizeof(cmdBufAllocInfo));
  cmdBufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cmdBufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cmdBufAllocInfo.commandBufferCount = 1;
  cmdBufAllocInfo.commandPool = ComputeCmdPool;

  vkAllocateCommandBuffers(LogicalDevice, &cmdBufAllocInfo, &cmdBuffer);

  VkCommandBufferBeginInfo cmdBufferBegin;
  memset(&cmdBufferBegin, 0, sizeof(cmdBufferBegin));
  cmdBufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  cmdBufferBegin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(cmdBuffer, &cmdBufferBegin);

  VkBufferCopy buffCopy;
  buffCopy.srcOffset = 0;
  buffCopy.dstOffset = 0;
  buffCopy.size = size;

  vkCmdCopyBuffer(cmdBuffer, srcBuffer, dstBuffer, 1, &buffCopy);
  vkEndCommandBuffer(cmdBuffer);

  VkSubmitInfo submitInfo;
  memset(&submitInfo, 0, sizeof(submitInfo));
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &cmdBuffer;

  vkQueueSubmit(ComputingQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(ComputingQueue);
  vkFreeCommandBuffers(LogicalDevice, ComputeCmdPool, 1, &cmdBuffer);
}

void CopyToLocalBuffer(void* data, uint32_t size, VkBuffer buffer)
{
  VkDeviceMemory tempMemory;
  VkBuffer tempBuffer = CreateBufferAndMemory(size, &tempMemory, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

  CopyData(data, size, 1, tempMemory);
  CopyBufferToBuffer(tempBuffer, buffer, size);
  vkDestroyBuffer(LogicalDevice, tempBuffer, NULL);
  vkFreeMemory(LogicalDevice, tempMemory, NULL);
}

void CopyToInputBuffer(void* data, uint32_t size)
{
  // CopyData(data, size, 1, InputBufferMemory);
  CopyToLocalBuffer(data, size, InputBuffer);
}

void CopyFromOutputBuffer(void* data, uint32_t size)
{
  CopyData(data, size, 0, OutputBufferMemory);
}

void CopyToOutputBuffer(void* data, uint32_t size)
{
  CopyData(data, size, 1, OutputBufferMemory);
}
