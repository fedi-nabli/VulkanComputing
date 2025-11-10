#ifndef __PIPELINE_H_
#define __PIPELINE_H_

#include <vulkan/vulkan.h>

extern VkPipeline Pipeline;
extern VkPipelineLayout PipelineLayout;

void CreatePipeline();
void DestroyPipeline();

#endif
