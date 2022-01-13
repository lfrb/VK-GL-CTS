/* WARNING: This is auto-generated file. Do not modify, since changes will
 * be lost! Modify the generating script instead.
 * This file was generated by /scripts/gen_framework.py
 */
Move<VkInstance>					createInstance						(const PlatformInterface& vk, const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkDevice>						createDevice						(const PlatformInterface& vkp, VkInstance instance, const InstanceInterface& vk, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkDeviceMemory>				allocateMemory						(const DeviceInterface& vk, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkFence>						createFence							(const DeviceInterface& vk, VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSemaphore>					createSemaphore						(const DeviceInterface& vk, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkEvent>						createEvent							(const DeviceInterface& vk, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkQueryPool>					createQueryPool						(const DeviceInterface& vk, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkBuffer>						createBuffer						(const DeviceInterface& vk, VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkBufferView>					createBufferView					(const DeviceInterface& vk, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkImage>						createImage							(const DeviceInterface& vk, VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkImageView>					createImageView						(const DeviceInterface& vk, VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkShaderModule>				createShaderModule					(const DeviceInterface& vk, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkPipelineCache>				createPipelineCache					(const DeviceInterface& vk, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkPipelineLayout>				createPipelineLayout				(const DeviceInterface& vk, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSampler>						createSampler						(const DeviceInterface& vk, VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkDescriptorSetLayout>			createDescriptorSetLayout			(const DeviceInterface& vk, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkDescriptorPool>				createDescriptorPool				(const DeviceInterface& vk, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkFramebuffer>					createFramebuffer					(const DeviceInterface& vk, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkRenderPass>					createRenderPass					(const DeviceInterface& vk, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkCommandPool>					createCommandPool					(const DeviceInterface& vk, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSamplerYcbcrConversion>		createSamplerYcbcrConversion		(const DeviceInterface& vk, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkDescriptorUpdateTemplate>	createDescriptorUpdateTemplate		(const DeviceInterface& vk, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkRenderPass>					createRenderPass2					(const DeviceInterface& vk, VkDevice device, const VkRenderPassCreateInfo2* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSwapchainKHR>				createSwapchainKHR					(const DeviceInterface& vk, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createDisplayPlaneSurfaceKHR		(const InstanceInterface& vk, VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSwapchainKHR>				createSharedSwapchainsKHR			(const DeviceInterface& vk, VkDevice device, deUint32 swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkDeferredOperationKHR>		createDeferredOperationKHR			(const DeviceInterface& vk, VkDevice device, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkDebugReportCallbackEXT>		createDebugReportCallbackEXT		(const InstanceInterface& vk, VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkCuModuleNVX>					createCuModuleNVX					(const DeviceInterface& vk, VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkCuFunctionNVX>				createCuFunctionNVX					(const DeviceInterface& vk, VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkDebugUtilsMessengerEXT>		createDebugUtilsMessengerEXT		(const InstanceInterface& vk, VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkValidationCacheEXT>			createValidationCacheEXT			(const DeviceInterface& vk, VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkAccelerationStructureNV>		createAccelerationStructureNV		(const DeviceInterface& vk, VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createHeadlessSurfaceEXT			(const InstanceInterface& vk, VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkIndirectCommandsLayoutNV>	createIndirectCommandsLayoutNV		(const DeviceInterface& vk, VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkPrivateDataSlotEXT>			createPrivateDataSlotEXT			(const DeviceInterface& vk, VkDevice device, const VkPrivateDataSlotCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkAccelerationStructureKHR>	createAccelerationStructureKHR		(const DeviceInterface& vk, VkDevice device, const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createAndroidSurfaceKHR				(const InstanceInterface& vk, VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkVideoSessionKHR>				createVideoSessionKHR				(const DeviceInterface& vk, VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkVideoSessionParametersKHR>	createVideoSessionParametersKHR		(const DeviceInterface& vk, VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createImagePipeSurfaceFUCHSIA		(const InstanceInterface& vk, VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createStreamDescriptorSurfaceGGP	(const InstanceInterface& vk, VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createIOSSurfaceMVK					(const InstanceInterface& vk, VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createMacOSSurfaceMVK				(const InstanceInterface& vk, VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createMetalSurfaceEXT				(const InstanceInterface& vk, VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createViSurfaceNN					(const InstanceInterface& vk, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createWaylandSurfaceKHR				(const InstanceInterface& vk, VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createWin32SurfaceKHR				(const InstanceInterface& vk, VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createXcbSurfaceKHR					(const InstanceInterface& vk, VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
Move<VkSurfaceKHR>					createXlibSurfaceKHR				(const InstanceInterface& vk, VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator = DE_NULL);
