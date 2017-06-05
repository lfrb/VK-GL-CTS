/* WARNING: This is auto-generated file. Do not modify, since changes will
 * be lost! Modify the generating script instead.
 */
DestroyDeviceFunc						destroyDevice;
GetDeviceQueueFunc						getDeviceQueue;
QueueSubmitFunc							queueSubmit;
QueueWaitIdleFunc						queueWaitIdle;
DeviceWaitIdleFunc						deviceWaitIdle;
AllocateMemoryFunc						allocateMemory;
FreeMemoryFunc							freeMemory;
MapMemoryFunc							mapMemory;
UnmapMemoryFunc							unmapMemory;
FlushMappedMemoryRangesFunc				flushMappedMemoryRanges;
InvalidateMappedMemoryRangesFunc		invalidateMappedMemoryRanges;
GetDeviceMemoryCommitmentFunc			getDeviceMemoryCommitment;
BindBufferMemoryFunc					bindBufferMemory;
BindImageMemoryFunc						bindImageMemory;
GetBufferMemoryRequirementsFunc			getBufferMemoryRequirements;
GetImageMemoryRequirementsFunc			getImageMemoryRequirements;
GetImageSparseMemoryRequirementsFunc	getImageSparseMemoryRequirements;
QueueBindSparseFunc						queueBindSparse;
CreateFenceFunc							createFence;
DestroyFenceFunc						destroyFence;
ResetFencesFunc							resetFences;
GetFenceStatusFunc						getFenceStatus;
WaitForFencesFunc						waitForFences;
CreateSemaphoreFunc						createSemaphore;
DestroySemaphoreFunc					destroySemaphore;
CreateEventFunc							createEvent;
DestroyEventFunc						destroyEvent;
GetEventStatusFunc						getEventStatus;
SetEventFunc							setEvent;
ResetEventFunc							resetEvent;
CreateQueryPoolFunc						createQueryPool;
DestroyQueryPoolFunc					destroyQueryPool;
GetQueryPoolResultsFunc					getQueryPoolResults;
CreateBufferFunc						createBuffer;
DestroyBufferFunc						destroyBuffer;
CreateBufferViewFunc					createBufferView;
DestroyBufferViewFunc					destroyBufferView;
CreateImageFunc							createImage;
DestroyImageFunc						destroyImage;
GetImageSubresourceLayoutFunc			getImageSubresourceLayout;
CreateImageViewFunc						createImageView;
DestroyImageViewFunc					destroyImageView;
CreateShaderModuleFunc					createShaderModule;
DestroyShaderModuleFunc					destroyShaderModule;
CreatePipelineCacheFunc					createPipelineCache;
DestroyPipelineCacheFunc				destroyPipelineCache;
GetPipelineCacheDataFunc				getPipelineCacheData;
MergePipelineCachesFunc					mergePipelineCaches;
CreateGraphicsPipelinesFunc				createGraphicsPipelines;
CreateComputePipelinesFunc				createComputePipelines;
DestroyPipelineFunc						destroyPipeline;
CreatePipelineLayoutFunc				createPipelineLayout;
DestroyPipelineLayoutFunc				destroyPipelineLayout;
CreateSamplerFunc						createSampler;
DestroySamplerFunc						destroySampler;
CreateDescriptorSetLayoutFunc			createDescriptorSetLayout;
DestroyDescriptorSetLayoutFunc			destroyDescriptorSetLayout;
CreateDescriptorPoolFunc				createDescriptorPool;
DestroyDescriptorPoolFunc				destroyDescriptorPool;
ResetDescriptorPoolFunc					resetDescriptorPool;
AllocateDescriptorSetsFunc				allocateDescriptorSets;
FreeDescriptorSetsFunc					freeDescriptorSets;
UpdateDescriptorSetsFunc				updateDescriptorSets;
CreateFramebufferFunc					createFramebuffer;
DestroyFramebufferFunc					destroyFramebuffer;
CreateRenderPassFunc					createRenderPass;
DestroyRenderPassFunc					destroyRenderPass;
GetRenderAreaGranularityFunc			getRenderAreaGranularity;
CreateCommandPoolFunc					createCommandPool;
DestroyCommandPoolFunc					destroyCommandPool;
ResetCommandPoolFunc					resetCommandPool;
AllocateCommandBuffersFunc				allocateCommandBuffers;
FreeCommandBuffersFunc					freeCommandBuffers;
BeginCommandBufferFunc					beginCommandBuffer;
EndCommandBufferFunc					endCommandBuffer;
ResetCommandBufferFunc					resetCommandBuffer;
CmdBindPipelineFunc						cmdBindPipeline;
CmdSetViewportFunc						cmdSetViewport;
CmdSetScissorFunc						cmdSetScissor;
CmdSetLineWidthFunc						cmdSetLineWidth;
CmdSetDepthBiasFunc						cmdSetDepthBias;
CmdSetBlendConstantsFunc				cmdSetBlendConstants;
CmdSetDepthBoundsFunc					cmdSetDepthBounds;
CmdSetStencilCompareMaskFunc			cmdSetStencilCompareMask;
CmdSetStencilWriteMaskFunc				cmdSetStencilWriteMask;
CmdSetStencilReferenceFunc				cmdSetStencilReference;
CmdBindDescriptorSetsFunc				cmdBindDescriptorSets;
CmdBindIndexBufferFunc					cmdBindIndexBuffer;
CmdBindVertexBuffersFunc				cmdBindVertexBuffers;
CmdDrawFunc								cmdDraw;
CmdDrawIndexedFunc						cmdDrawIndexed;
CmdDrawIndirectFunc						cmdDrawIndirect;
CmdDrawIndexedIndirectFunc				cmdDrawIndexedIndirect;
CmdDispatchFunc							cmdDispatch;
CmdDispatchIndirectFunc					cmdDispatchIndirect;
CmdCopyBufferFunc						cmdCopyBuffer;
CmdCopyImageFunc						cmdCopyImage;
CmdBlitImageFunc						cmdBlitImage;
CmdCopyBufferToImageFunc				cmdCopyBufferToImage;
CmdCopyImageToBufferFunc				cmdCopyImageToBuffer;
CmdUpdateBufferFunc						cmdUpdateBuffer;
CmdFillBufferFunc						cmdFillBuffer;
CmdClearColorImageFunc					cmdClearColorImage;
CmdClearDepthStencilImageFunc			cmdClearDepthStencilImage;
CmdClearAttachmentsFunc					cmdClearAttachments;
CmdResolveImageFunc						cmdResolveImage;
CmdSetEventFunc							cmdSetEvent;
CmdResetEventFunc						cmdResetEvent;
CmdWaitEventsFunc						cmdWaitEvents;
CmdPipelineBarrierFunc					cmdPipelineBarrier;
CmdBeginQueryFunc						cmdBeginQuery;
CmdEndQueryFunc							cmdEndQuery;
CmdResetQueryPoolFunc					cmdResetQueryPool;
CmdWriteTimestampFunc					cmdWriteTimestamp;
CmdCopyQueryPoolResultsFunc				cmdCopyQueryPoolResults;
CmdPushConstantsFunc					cmdPushConstants;
CmdBeginRenderPassFunc					cmdBeginRenderPass;
CmdNextSubpassFunc						cmdNextSubpass;
CmdEndRenderPassFunc					cmdEndRenderPass;
CmdExecuteCommandsFunc					cmdExecuteCommands;
CreateSwapchainKHRFunc					createSwapchainKHR;
DestroySwapchainKHRFunc					destroySwapchainKHR;
GetSwapchainImagesKHRFunc				getSwapchainImagesKHR;
AcquireNextImageKHRFunc					acquireNextImageKHR;
QueuePresentKHRFunc						queuePresentKHR;
CreateSharedSwapchainsKHRFunc			createSharedSwapchainsKHR;
TrimCommandPoolKHRFunc					trimCommandPoolKHR;
CmdPushDescriptorSetKHRFunc				cmdPushDescriptorSetKHR;
CreateDescriptorUpdateTemplateKHRFunc	createDescriptorUpdateTemplateKHR;
DestroyDescriptorUpdateTemplateKHRFunc	destroyDescriptorUpdateTemplateKHR;
UpdateDescriptorSetWithTemplateKHRFunc	updateDescriptorSetWithTemplateKHR;
CmdPushDescriptorSetWithTemplateKHRFunc	cmdPushDescriptorSetWithTemplateKHR;
GetSwapchainStatusKHRFunc				getSwapchainStatusKHR;
GetRefreshCycleDurationGOOGLEFunc		getRefreshCycleDurationGOOGLE;
GetPastPresentationTimingGOOGLEFunc		getPastPresentationTimingGOOGLE;
