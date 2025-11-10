#ifndef __COMPUTE_H_
#define __COMPUTE_H_

#include <vulkan/vulkan.h>

extern VkDescriptorSet DescriptorSet;

int Compute();
void CreateDescriptorSet();
void PrepareCommandBuffer();

#endif
