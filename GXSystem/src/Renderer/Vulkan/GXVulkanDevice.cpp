#include "GXVulkanDevice.h"

#include <cstring>

namespace gx {

    static bool VulkanIsDeviceMeetsRequirements(VkPhysicalDevice Device, VkSurfaceKHR Surface, VkPhysicalDeviceProperties *Properties, VkPhysicalDeviceFeatures *Features, VkPhysicalDeviceMemoryProperties *Memory, GXVulkanDeviceRequirements* Requirements, GXVulkanQueueFamilyIndices *OutQueueFamilyIndices)
    {
        if (Requirements->is_discrete && Properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            return false;
        }

        if (Requirements->has_sampler_anisotropy && !Features->samplerAnisotropy)
        {
            return false;
        }

        GXuint32 queueFamilyPropertiesCount;
        vkGetPhysicalDeviceQueueFamilyProperties(Device, &queueFamilyPropertiesCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(Device, &queueFamilyPropertiesCount, queueFamilyProperties.data());

        GXuint32 minTransferScore = UINT_MAX;

        for (GXuint32 i = 0; i < queueFamilyPropertiesCount; i++)
        {
            GXuint32 transferScore = 0;

            if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                OutQueueFamilyIndices->graphics_queue_family_index = i;
                transferScore++;
            }

            if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                OutQueueFamilyIndices->compute_queue_family_index = i;
                transferScore++;
            }

            if (queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                if (transferScore < minTransferScore) {
                    minTransferScore = transferScore;
                    OutQueueFamilyIndices->transfer_queue_family_index = i;
                }
            }

            VkBool32 supported;
            vkGetPhysicalDeviceSurfaceSupportKHR(Device, i, Surface, &supported);

            if (supported)
            {
                OutQueueFamilyIndices->present_queue_family_index = i;
            }
        }

        if (Requirements->has_graphics_queue && OutQueueFamilyIndices->graphics_queue_family_index == -1 || Requirements->has_present_queue && OutQueueFamilyIndices->present_queue_family_index == -1 || Requirements->has_compute_queue && OutQueueFamilyIndices->compute_queue_family_index == -1 || Requirements->has_transfer_queue && OutQueueFamilyIndices->transfer_queue_family_index == -1)
        {
            return false;
        }

        if (Requirements->extensions.size())
        {
            GXuint32 extensionCount;
            vkEnumerateDeviceExtensionProperties(Device, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(Device, nullptr, &extensionCount, extensions.data());

            for (auto& extension : Requirements->extensions)
            {
                bool supported = false;

                for (auto& [name, version] : extensions)
                {
                    if (strcmp(name, extension) == 0)
                    {
                        supported = true;
                        break;
                    }
                }

                if (!supported)
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool VulkanCreateDevice(GXVulkanContext* Context, GXVulkanDeviceRequirements* Requirements, GXVulkanDevice *outDevice)
    {
        GXuint32 physicalDeviceCount = 0;
        vkEnumeratePhysicalDevices(Context->instance, &physicalDeviceCount, nullptr);

        if (physicalDeviceCount == 0)
        {
            return false;
        }

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(Context->instance, &physicalDeviceCount, physicalDevices.data());

        bool is_device_selected = false;

        for (GXuint32 i = 0; i < physicalDeviceCount; i++)
        {
            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceFeatures features;
            VkPhysicalDeviceMemoryProperties memory;

            vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);
            vkGetPhysicalDeviceFeatures(physicalDevices[i], &features);
            vkGetPhysicalDeviceMemoryProperties(physicalDevices[i], &memory);

            GXVulkanQueueFamilyIndices queueFamilyIndices = { -1, -1, -1, -1 };

            if (VulkanIsDeviceMeetsRequirements(physicalDevices[i], Context->surface, &properties, &features, &memory, Requirements, &queueFamilyIndices))
            {
                Context->device.physical = physicalDevices[i];
                Context->device.properties = properties;
                Context->device.features = features;
                Context->device.memory = memory;
                Context->device.queue_family_indices = queueFamilyIndices;
                is_device_selected = true;
                break;
            }
        }

        if (!is_device_selected)
        {
            return false;
        }

        GXFloat queuePriority = 1.0f;

        std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

        VkDeviceQueueCreateInfo graphics_queue_create_info = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        graphics_queue_create_info.queueFamilyIndex = Context->device.queue_family_indices.graphics_queue_family_index;
        graphics_queue_create_info.queueCount = 1;
        graphics_queue_create_info.pQueuePriorities = &queuePriority;

        deviceQueueCreateInfos.push_back(graphics_queue_create_info);

        if (Context->device.queue_family_indices.compute_queue_family_index != Context->device.queue_family_indices.graphics_queue_family_index)
        {
            VkDeviceQueueCreateInfo compute_queue_create_info = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
            compute_queue_create_info.queueFamilyIndex = Context->device.queue_family_indices.compute_queue_family_index;
            compute_queue_create_info.queueCount = 1;
            compute_queue_create_info.pQueuePriorities = &queuePriority;
            deviceQueueCreateInfos.push_back(compute_queue_create_info);
        }

        if (Context->device.queue_family_indices.present_queue_family_index != Context->device.queue_family_indices.graphics_queue_family_index && Context->device.queue_family_indices.present_queue_family_index != Context->device.queue_family_indices.compute_queue_family_index)
        {
            VkDeviceQueueCreateInfo present_queue_create_info = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
            present_queue_create_info.queueFamilyIndex = Context->device.queue_family_indices.present_queue_family_index;
            present_queue_create_info.queueCount = 1;
            present_queue_create_info.pQueuePriorities = &queuePriority;
            deviceQueueCreateInfos.push_back(present_queue_create_info);
        }

        if (Context->device.queue_family_indices.transfer_queue_family_index != Context->device.queue_family_indices.graphics_queue_family_index && Context->device.queue_family_indices.transfer_queue_family_index != Context->device.queue_family_indices.compute_queue_family_index && Context->device.queue_family_indices.transfer_queue_family_index != Context->device.queue_family_indices.present_queue_family_index)
        {
            VkDeviceQueueCreateInfo transfer_queue_create_info = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
            transfer_queue_create_info.queueFamilyIndex = Context->device.queue_family_indices.transfer_queue_family_index;
            transfer_queue_create_info.queueCount = 1;
            transfer_queue_create_info.pQueuePriorities = &queuePriority;
            deviceQueueCreateInfos.push_back(transfer_queue_create_info);
        }

        VkDeviceCreateInfo deviceCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
        deviceCreateInfo.queueCreateInfoCount = deviceQueueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures = &Context->device.features;
        deviceCreateInfo.enabledExtensionCount = Requirements->extensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = Requirements->extensions.data();

        VkResult result = vkCreateDevice(Context->device.physical, &deviceCreateInfo, nullptr, &Context->device.logical);

        if (result != VK_SUCCESS)
        {
            return false;
        }

        vkGetDeviceQueue(Context->device.logical, Context->device.queue_family_indices.graphics_queue_family_index, 0, &Context->device.graphics_queue);
        vkGetDeviceQueue(Context->device.logical, Context->device.queue_family_indices.compute_queue_family_index, 0, &Context->device.compute_queue);
        vkGetDeviceQueue(Context->device.logical, Context->device.queue_family_indices.present_queue_family_index, 0, &Context->device.present_queue);
        vkGetDeviceQueue(Context->device.logical, Context->device.queue_family_indices.transfer_queue_family_index, 0, &Context->device.transfer_queue);

        return true;
    }

    void VulkanDestroyDevice(GXVulkanDevice *Device)
    {
        vkDestroyDevice(Device->logical, nullptr);
    }
}