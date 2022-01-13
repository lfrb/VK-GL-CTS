/* WARNING: This is auto-generated file. Do not modify, since changes will
 * be lost! Modify the generating script instead.
 * This file was generated by /scripts/gen_framework.py
 */
virtual void		destroyInstance													(VkInstance instance, const VkAllocationCallbacks* pAllocator) const;
virtual VkResult	enumeratePhysicalDevices										(VkInstance instance, deUint32* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) const;
virtual void		getPhysicalDeviceFeatures										(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) const;
virtual void		getPhysicalDeviceFormatProperties								(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) const;
virtual VkResult	getPhysicalDeviceImageFormatProperties							(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) const;
virtual void		getPhysicalDeviceProperties										(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) const;
virtual void		getPhysicalDeviceQueueFamilyProperties							(VkPhysicalDevice physicalDevice, deUint32* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) const;
virtual void		getPhysicalDeviceMemoryProperties								(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) const;
virtual VkResult	createDevice													(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) const;
virtual VkResult	enumerateDeviceExtensionProperties								(VkPhysicalDevice physicalDevice, const char* pLayerName, deUint32* pPropertyCount, VkExtensionProperties* pProperties) const;
virtual VkResult	enumerateDeviceLayerProperties									(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkLayerProperties* pProperties) const;
virtual void		getPhysicalDeviceSparseImageFormatProperties					(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, deUint32* pPropertyCount, VkSparseImageFormatProperties* pProperties) const;
virtual VkResult	enumeratePhysicalDeviceGroups									(VkInstance instance, deUint32* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) const;
virtual void		getPhysicalDeviceFeatures2										(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) const;
virtual void		getPhysicalDeviceProperties2									(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) const;
virtual void		getPhysicalDeviceFormatProperties2								(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) const;
virtual VkResult	getPhysicalDeviceImageFormatProperties2							(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) const;
virtual void		getPhysicalDeviceQueueFamilyProperties2							(VkPhysicalDevice physicalDevice, deUint32* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) const;
virtual void		getPhysicalDeviceMemoryProperties2								(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) const;
virtual void		getPhysicalDeviceSparseImageFormatProperties2					(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, deUint32* pPropertyCount, VkSparseImageFormatProperties2* pProperties) const;
virtual void		getPhysicalDeviceExternalBufferProperties						(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) const;
virtual void		getPhysicalDeviceExternalFenceProperties						(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) const;
virtual void		getPhysicalDeviceExternalSemaphoreProperties					(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) const;
virtual void		destroySurfaceKHR												(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) const;
virtual VkResult	getPhysicalDeviceSurfaceSupportKHR								(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) const;
virtual VkResult	getPhysicalDeviceSurfaceCapabilitiesKHR							(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) const;
virtual VkResult	getPhysicalDeviceSurfaceFormatsKHR								(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, deUint32* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) const;
virtual VkResult	getPhysicalDeviceSurfacePresentModesKHR							(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, deUint32* pPresentModeCount, VkPresentModeKHR* pPresentModes) const;
virtual VkResult	getPhysicalDevicePresentRectanglesKHR							(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, deUint32* pRectCount, VkRect2D* pRects) const;
virtual VkResult	getPhysicalDeviceDisplayPropertiesKHR							(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkDisplayPropertiesKHR* pProperties) const;
virtual VkResult	getPhysicalDeviceDisplayPlanePropertiesKHR						(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) const;
virtual VkResult	getDisplayPlaneSupportedDisplaysKHR								(VkPhysicalDevice physicalDevice, deUint32 planeIndex, deUint32* pDisplayCount, VkDisplayKHR* pDisplays) const;
virtual VkResult	getDisplayModePropertiesKHR										(VkPhysicalDevice physicalDevice, VkDisplayKHR display, deUint32* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) const;
virtual VkResult	createDisplayModeKHR											(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) const;
virtual VkResult	getDisplayPlaneCapabilitiesKHR									(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, deUint32 planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) const;
virtual VkResult	createDisplayPlaneSurfaceKHR									(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkResult	enumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR	(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, deUint32* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions) const;
virtual void		getPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR			(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, deUint32* pNumPasses) const;
virtual VkResult	getPhysicalDeviceSurfaceCapabilities2KHR						(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) const;
virtual VkResult	getPhysicalDeviceSurfaceFormats2KHR								(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, deUint32* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) const;
virtual VkResult	getPhysicalDeviceDisplayProperties2KHR							(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkDisplayProperties2KHR* pProperties) const;
virtual VkResult	getPhysicalDeviceDisplayPlaneProperties2KHR						(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) const;
virtual VkResult	getDisplayModeProperties2KHR									(VkPhysicalDevice physicalDevice, VkDisplayKHR display, deUint32* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) const;
virtual VkResult	getDisplayPlaneCapabilities2KHR									(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) const;
virtual VkResult	getPhysicalDeviceFragmentShadingRatesKHR						(VkPhysicalDevice physicalDevice, deUint32* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates) const;
virtual VkResult	createDebugReportCallbackEXT									(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) const;
virtual void		destroyDebugReportCallbackEXT									(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) const;
virtual void		debugReportMessageEXT											(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, deUint64 object, deUintptr location, deInt32 messageCode, const char* pLayerPrefix, const char* pMessage) const;
virtual VkResult	getPhysicalDeviceExternalImageFormatPropertiesNV				(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) const;
virtual VkResult	releaseDisplayEXT												(VkPhysicalDevice physicalDevice, VkDisplayKHR display) const;
virtual VkResult	getPhysicalDeviceSurfaceCapabilities2EXT						(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) const;
virtual VkResult	createDebugUtilsMessengerEXT									(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) const;
virtual void		destroyDebugUtilsMessengerEXT									(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator) const;
virtual void		submitDebugUtilsMessageEXT										(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) const;
virtual void		getPhysicalDeviceMultisamplePropertiesEXT						(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) const;
virtual VkResult	getPhysicalDeviceCalibrateableTimeDomainsEXT					(VkPhysicalDevice physicalDevice, deUint32* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains) const;
virtual VkResult	getPhysicalDeviceToolPropertiesEXT								(VkPhysicalDevice physicalDevice, deUint32* pToolCount, VkPhysicalDeviceToolPropertiesEXT* pToolProperties) const;
virtual VkResult	getPhysicalDeviceCooperativeMatrixPropertiesNV					(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties) const;
virtual VkResult	getPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV	(VkPhysicalDevice physicalDevice, deUint32* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations) const;
virtual VkResult	createHeadlessSurfaceEXT										(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkResult	acquireDrmDisplayEXT											(VkPhysicalDevice physicalDevice, deInt32 drmFd, VkDisplayKHR display) const;
virtual VkResult	getDrmDisplayEXT												(VkPhysicalDevice physicalDevice, deInt32 drmFd, deUint32 connectorId, VkDisplayKHR* display) const;
virtual VkResult	acquireWinrtDisplayNV											(VkPhysicalDevice physicalDevice, VkDisplayKHR display) const;
virtual VkResult	getWinrtDisplayNV												(VkPhysicalDevice physicalDevice, deUint32 deviceRelativeId, VkDisplayKHR* pDisplay) const;
virtual VkResult	createAndroidSurfaceKHR											(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkResult	getPhysicalDeviceVideoCapabilitiesKHR							(VkPhysicalDevice physicalDevice, const VkVideoProfileKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities) const;
virtual VkResult	getPhysicalDeviceVideoFormatPropertiesKHR						(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, deUint32* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties) const;
virtual VkResult	createImagePipeSurfaceFUCHSIA									(VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkResult	createStreamDescriptorSurfaceGGP								(VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkResult	createIOSSurfaceMVK												(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkResult	createMacOSSurfaceMVK											(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkResult	createMetalSurfaceEXT											(VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkResult	createViSurfaceNN												(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkResult	createWaylandSurfaceKHR											(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkBool32	getPhysicalDeviceWaylandPresentationSupportKHR					(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, pt::WaylandDisplayPtr display) const;
virtual VkResult	createWin32SurfaceKHR											(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkBool32	getPhysicalDeviceWin32PresentationSupportKHR					(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex) const;
virtual VkResult	getPhysicalDeviceSurfacePresentModes2EXT						(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, deUint32* pPresentModeCount, VkPresentModeKHR* pPresentModes) const;
virtual VkResult	createXcbSurfaceKHR												(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkBool32	getPhysicalDeviceXcbPresentationSupportKHR						(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, pt::XcbConnectionPtr connection, pt::XcbVisualid visual_id) const;
virtual VkResult	createXlibSurfaceKHR											(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
virtual VkBool32	getPhysicalDeviceXlibPresentationSupportKHR						(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, pt::XlibDisplayPtr dpy, pt::XlibVisualID visualID) const;
virtual VkResult	acquireXlibDisplayEXT											(VkPhysicalDevice physicalDevice, pt::XlibDisplayPtr dpy, VkDisplayKHR display) const;
virtual VkResult	getRandROutputDisplayEXT										(VkPhysicalDevice physicalDevice, pt::XlibDisplayPtr dpy, pt::RROutput rrOutput, VkDisplayKHR* pDisplay) const;
