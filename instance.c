#include "instance.h"

#include <stdio.h>
#include <string.h>
#include <vulkan/vulkan.h>

VkInstance Instance = VK_NULL_HANDLE;
VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;

void GetPhysicalDevice()
{
  VkPhysicalDevice devices[100];
  uint32_t count = 100;

  if (vkEnumeratePhysicalDevices(Instance, &count, devices) != VK_SUCCESS)
  {
    printf("Enumeratinf physical devices failed\n");
    return;
  }

  PhysicalDevice = devices[0];
}

void CreateInstance()
{
  VkInstanceCreateInfo instanceCreateInfo;
  memset(&instanceCreateInfo, 0, sizeof(instanceCreateInfo));
  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

  if (vkCreateInstance(&instanceCreateInfo, NULL, &Instance) != VK_SUCCESS)
  {
    printf("Instance not created\n");
    return;
  }
}
