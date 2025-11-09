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
    printf("Enumerating physical devices failed\n");
    return;
  }

  PhysicalDevice = devices[0];

  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;

  vkGetPhysicalDeviceProperties(PhysicalDevice, &deviceProperties);
  vkGetPhysicalDeviceFeatures(PhysicalDevice, &deviceFeatures);
}

void CreateInstance()
{
  const char* extensions[] = {
    "VK_KHR_portability_enumeration"
  };

   const char* layers[] = {
    "VK_LAYER_KHRONOS_validation",
   };

  VkApplicationInfo appInfo;
  memset(&appInfo, 0, sizeof(appInfo));
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "VulkanCompute";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;
  
  VkInstanceCreateInfo instanceCreateInfo;
  memset(&instanceCreateInfo, 0, sizeof(instanceCreateInfo));
  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pApplicationInfo = &appInfo;
  instanceCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  instanceCreateInfo.enabledExtensionCount = 1;
  instanceCreateInfo.ppEnabledExtensionNames = extensions;
  instanceCreateInfo.enabledLayerCount = 1;
  instanceCreateInfo.ppEnabledLayerNames = layers;

  VkResult result = vkCreateInstance(&instanceCreateInfo, NULL, &Instance);
  if (result != VK_SUCCESS)
  {
    printf("Instance not created, VkResult: %d\n", result);
    return;
  }
}
