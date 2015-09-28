/*------------------------------------------------------------------------
 * Vulkan Conformance Tests
 * ------------------------
 *
 * Copyright (c) 2015 The Khronos Group Inc.
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice(s) and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * The Materials are Confidential Information as defined by the
 * Khronos Membership Agreement until designated non-confidential by Khronos,
 * at which point this condition clause shall be removed.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//*!
 * \file
 * \brief Vulkan ShaderRenderCase
 *//*--------------------------------------------------------------------*/

#include "vktShaderRenderCase.hpp"

#include "tcuImageCompare.hpp"
#include "tcuSurface.hpp"
#include "tcuVector.hpp"
#include "tcuTestLog.hpp"
#include "tcuTextureUtil.hpp"

#include "deMath.h"
#include "deUniquePtr.hpp"

#include "vkPlatform.hpp"
#include "vkStrUtil.hpp"
#include "vkRef.hpp"
#include "vkRefUtil.hpp"
#include "vkQueryUtil.hpp"
#include "vkDeviceUtil.hpp"

#include <vector>
#include <string>

namespace vkt
{
namespace shaderrendercase
{

using namespace vk;

static const int		GRID_SIZE			= 2;
static const int		MAX_RENDER_WIDTH	= 128;
static const int		MAX_RENDER_HEIGHT	= 112;
static const tcu::Vec4	DEFAULT_CLEAR_COLOR	= tcu::Vec4(0.125f, 0.25f, 0.5f, 1.0f);

// QuadGrid.

class QuadGrid
{
public:
										QuadGrid				(int								gridSize,
																 int								screenWidth,
																 int								screenHeight,
																 const tcu::Vec4&					constCoords,
																 const std::vector<tcu::Mat4>&		userAttribTransforms,
																 const std::vector<TextureBinding>&	textures);
										~QuadGrid				(void);

	int									getGridSize				(void) const { return m_gridSize; }
	int									getNumVertices			(void) const { return m_numVertices; }
	int									getNumTriangles			(void) const { return m_numTriangles; }
	const tcu::Vec4&					getConstCoords			(void) const { return m_constCoords; }
	const std::vector<tcu::Mat4>		getUserAttribTransforms	(void) const { return m_userAttribTransforms; }
	const std::vector<TextureBinding>&	getTextures				(void) const { return m_textures; }

	const tcu::Vec4*					getPositions			(void) const { return &m_positions[0]; }
	const float*						getAttribOne			(void) const { return &m_attribOne[0]; }
	const tcu::Vec4*					getCoords				(void) const { return &m_coords[0]; }
	const tcu::Vec4*					getUnitCoords			(void) const { return &m_unitCoords[0]; }

	const tcu::Vec4*					getUserAttrib			(int attribNdx) const { return &m_userAttribs[attribNdx][0]; }
	const deUint16*						getIndices				(void) const { return &m_indices[0]; }

	tcu::Vec4							getCoords				(float sx, float sy) const;
	tcu::Vec4							getUnitCoords			(float sx, float sy) const;

	int									getNumUserAttribs		(void) const { return (int)m_userAttribTransforms.size(); }
	tcu::Vec4							getUserAttrib			(int attribNdx, float sx, float sy) const;

private:
	int									m_gridSize;
	int									m_numVertices;
	int									m_numTriangles;
	tcu::Vec4							m_constCoords;
	std::vector<tcu::Mat4>				m_userAttribTransforms;

	std::vector<TextureBinding>			m_textures;

	std::vector<tcu::Vec4>				m_screenPos;
	std::vector<tcu::Vec4>				m_positions;
	std::vector<tcu::Vec4>				m_coords;		//!< Near-unit coordinates, roughly [-2.0 .. 2.0].
	std::vector<tcu::Vec4>				m_unitCoords;	//!< Positive-only coordinates [0.0 .. 1.5].
	std::vector<float>					m_attribOne;
	std::vector<tcu::Vec4>				m_userAttribs[ShaderEvalContext::MAX_TEXTURES];
	std::vector<deUint16>				m_indices;
};

QuadGrid::QuadGrid (int									gridSize,
					int									width,
					int									height,
					const tcu::Vec4&					constCoords,
					const std::vector<tcu::Mat4>&		userAttribTransforms,
					const std::vector<TextureBinding>&	textures)
	: m_gridSize				(gridSize)
	, m_numVertices				((gridSize + 1) * (gridSize + 1))
	, m_numTriangles			(gridSize * gridSize * 2)
	, m_constCoords				(constCoords)
	, m_userAttribTransforms	(userAttribTransforms)
	, m_textures				(textures)
{
	tcu::Vec4 viewportScale = tcu::Vec4((float)width, (float)height, 0.0f, 0.0f);

	// Compute vertices.
	m_screenPos.resize(m_numVertices);
	m_positions.resize(m_numVertices);
	m_coords.resize(m_numVertices);
	m_unitCoords.resize(m_numVertices);
	m_attribOne.resize(m_numVertices);

	// User attributes.
	for (int attrNdx = 0; attrNdx < DE_LENGTH_OF_ARRAY(m_userAttribs); attrNdx++)
		m_userAttribs[attrNdx].resize(m_numVertices);

	for (int y = 0; y < gridSize+1; y++)
	for (int x = 0; x < gridSize+1; x++)
	{
		float		sx			= (float)x / (float)gridSize;
		float		sy			= (float)y / (float)gridSize;
		float		fx			= 2.0f * sx - 1.0f;
		float		fy			= 2.0f * sy - 1.0f;
		int			vtxNdx		= ((y * (gridSize+1)) + x);

		m_positions[vtxNdx] 	= tcu::Vec4(fx, fy, 0.0f, 1.0f);
		m_coords[vtxNdx] 		= getCoords(sx, sy);
		m_unitCoords[vtxNdx]	= getUnitCoords(sx, sy);
		m_attribOne[vtxNdx] 	= 1.0f;

		m_screenPos[vtxNdx]		= tcu::Vec4(sx, sy, 0.0f, 1.0f) * viewportScale;

		for (int attribNdx = 0; attribNdx < getNumUserAttribs(); attribNdx++)
			m_userAttribs[attribNdx][vtxNdx] = getUserAttrib(attribNdx, sx, sy);
	}

	// Compute indices.
	m_indices.resize(3 * m_numTriangles);
	for (int y = 0; y < gridSize; y++)
	for (int x = 0; x < gridSize; x++)
	{
		int stride 				= gridSize + 1;
		int v00 				= (y * stride) + x;
		int v01 				= (y * stride) + x + 1;
		int v10 				= ((y+1) * stride) + x;
		int v11 				= ((y+1) * stride) + x + 1;

		int baseNdx 			= ((y * gridSize) + x) * 6;
		m_indices[baseNdx + 0]	= (deUint16)v10;
		m_indices[baseNdx + 1]	= (deUint16)v00;
		m_indices[baseNdx + 2]	= (deUint16)v01;

		m_indices[baseNdx + 3]	= (deUint16)v10;
		m_indices[baseNdx + 4]	= (deUint16)v01;
		m_indices[baseNdx + 5]	= (deUint16)v11;
	}
}

QuadGrid::~QuadGrid (void)
{
}

inline tcu::Vec4 QuadGrid::getCoords (float sx, float sy) const
{
	float fx = 2.0f * sx - 1.0f;
	float fy = 2.0f * sy - 1.0f;
	return tcu::Vec4(fx, fy, -fx + 0.33f*fy, -0.275f*fx - fy);
}

inline tcu::Vec4 QuadGrid::getUnitCoords (float sx, float sy) const
{
	return tcu::Vec4(sx, sy, 0.33f*sx + 0.5f*sy, 0.5f*sx + 0.25f*sy);
}

inline tcu::Vec4 QuadGrid::getUserAttrib (int attribNdx, float sx, float sy) const
{
	// homogeneous normalized screen-space coordinates
	return m_userAttribTransforms[attribNdx] * tcu::Vec4(sx, sy, 0.0f, 1.0f);
}

// TextureBinding

TextureBinding::TextureBinding (const Texture2D* tex2D, const tcu::Sampler& sampler)
	: m_type	(TYPE_2D)
	, m_sampler	(sampler)
{
	m_binding.tex2D = tex2D;
}

// ShaderEvalContext.

ShaderEvalContext::ShaderEvalContext (const QuadGrid& quadGrid)
	: constCoords	(quadGrid.getConstCoords())
	, isDiscarded	(false)
	, m_quadGrid	(quadGrid)
{
	const std::vector<TextureBinding>& bindings = m_quadGrid.getTextures();
	DE_ASSERT((int)bindings.size() <= MAX_TEXTURES);

	// Fill in texture array.
	for (int ndx = 0; ndx < (int)bindings.size(); ndx++)
	{
		const TextureBinding& binding = bindings[ndx];

		if (binding.getType() == TextureBinding::TYPE_NONE)
			continue;

		textures[ndx].sampler = binding.getSampler();

		switch (binding.getType())
		{
			case TextureBinding::TYPE_2D:		textures[ndx].tex2D			= &binding.get2D()->getRefTexture();		break;
			// \todo [2015-09-07 elecro] Add support for the other binding types
			/*
			case TextureBinding::TYPE_CUBE_MAP:	textures[ndx].texCube		= &binding.getCube()->getRefTexture();		break;
			case TextureBinding::TYPE_2D_ARRAY:	textures[ndx].tex2DArray	= &binding.get2DArray()->getRefTexture();	break;
			case TextureBinding::TYPE_3D:		textures[ndx].tex3D			= &binding.get3D()->getRefTexture();		break;
			*/
			default:
				TCU_THROW(InternalError, "Handling of texture binding type not implemented");
		}
	}
}

ShaderEvalContext::~ShaderEvalContext (void)
{
}

void ShaderEvalContext::reset (float sx, float sy)
{
	// Clear old values
	color		= tcu::Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	isDiscarded	= false;

	// Compute coords
	coords		= m_quadGrid.getCoords(sx, sy);
	unitCoords	= m_quadGrid.getUnitCoords(sx, sy);

	// Compute user attributes.
	int numAttribs = m_quadGrid.getNumUserAttribs();
	DE_ASSERT(numAttribs <= MAX_USER_ATTRIBS);
	for (int attribNdx = 0; attribNdx < numAttribs; attribNdx++)
		in[attribNdx] = m_quadGrid.getUserAttrib(attribNdx, sx, sy);
}

tcu::Vec4 ShaderEvalContext::texture2D (int unitNdx, const tcu::Vec2& texCoords)
{
	if (textures[unitNdx].tex2D)
		return textures[unitNdx].tex2D->sample(textures[unitNdx].sampler, texCoords.x(), texCoords.y(), 0.0f);
	else
	return tcu::Vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

// ShaderEvaluator.

ShaderEvaluator::ShaderEvaluator (void)
	: m_evalFunc(DE_NULL)
{
}

ShaderEvaluator::ShaderEvaluator (ShaderEvalFunc evalFunc)
	: m_evalFunc(evalFunc)
{
}

ShaderEvaluator::~ShaderEvaluator (void)
{
}

void ShaderEvaluator::evaluate (ShaderEvalContext& ctx)
{
	DE_ASSERT(m_evalFunc);
	m_evalFunc(ctx);
}

// UniformSetup.

UniformSetup::UniformSetup (void)
	: m_setupFunc(DE_NULL)
{
}

UniformSetup::UniformSetup (UniformSetupFunc setupFunc)
	: m_setupFunc(setupFunc)
{
}

UniformSetup::~UniformSetup (void)
{
}

void UniformSetup::setup (ShaderRenderCaseInstance& instance, const tcu::Vec4& constCoords)
{
	if (m_setupFunc)
		m_setupFunc(instance, constCoords);
}

// ShaderRenderCase.

ShaderRenderCase::ShaderRenderCase (tcu::TestContext&	testCtx,
									const std::string&	name,
									const std::string&	description,
									bool				isVertexCase,
									ShaderEvalFunc		evalFunc,
									UniformSetup*		uniformSetup,
									AttributeSetupFunc	attribFunc)
	: vkt::TestCase		(testCtx, name, description)
	, m_isVertexCase	(isVertexCase)
	, m_evaluator		(new ShaderEvaluator(evalFunc))
	, m_uniformSetup	(uniformSetup ? uniformSetup : new UniformSetup())
	, m_attribFunc		(attribFunc)
{}

ShaderRenderCase::ShaderRenderCase (tcu::TestContext&	testCtx,
									const std::string&	name,
									const std::string&	description,
									bool				isVertexCase,
									ShaderEvaluator*	evaluator,
									UniformSetup*		uniformSetup,
									AttributeSetupFunc	attribFunc)
	: vkt::TestCase		(testCtx, name, description)
	, m_isVertexCase	(isVertexCase)
	, m_evaluator		(evaluator)
	, m_uniformSetup	(uniformSetup ? uniformSetup : new UniformSetup())
	, m_attribFunc		(attribFunc)
{}

ShaderRenderCase::~ShaderRenderCase (void)
{
	delete m_evaluator;
	m_evaluator = DE_NULL;
	delete m_uniformSetup;
	m_uniformSetup = DE_NULL;
}

void ShaderRenderCase::initPrograms (vk::ProgramCollection<glu::ProgramSources>& programCollection) const
{
	programCollection.add("vert") << glu::VertexSource(m_vertShaderSource);
	programCollection.add("frag") << glu::FragmentSource(m_fragShaderSource);
}

TestInstance* ShaderRenderCase::createInstance (Context& context) const
{
	DE_ASSERT(m_evaluator != DE_NULL);
	DE_ASSERT(m_uniformSetup != DE_NULL);
	return new ShaderRenderCaseInstance(context, m_isVertexCase, *m_evaluator, *m_uniformSetup, m_attribFunc);
}

// ShaderRenderCaseInstance.

ShaderRenderCaseInstance::ShaderRenderCaseInstance (Context&			context,
													bool				isVertexCase,
													ShaderEvaluator&	evaluator,
													UniformSetup&		uniformSetup,
													AttributeSetupFunc	attribFunc)
	: vkt::TestInstance	(context)
	, m_clearColor		(DEFAULT_CLEAR_COLOR)
	, m_memAlloc		(m_context.getDeviceInterface(), m_context.getDevice(), getPhysicalDeviceMemoryProperties(m_context.getInstanceInterface(), m_context.getPhysicalDevice()))
	, m_isVertexCase	(isVertexCase)
	, m_evaluator		(evaluator)
	, m_uniformSetup	(uniformSetup)
	, m_attribFunc		(attribFunc)
	, m_renderSize		(100, 100)
	, m_colorFormat		(VK_FORMAT_R8G8B8A8_UNORM)
{
}

ShaderRenderCaseInstance::~ShaderRenderCaseInstance (void)
{
	const VkDevice			vkDevice	= m_context.getDevice();
	const DeviceInterface&	vk			= m_context.getDeviceInterface();

	for (size_t bufferNdx = 0; bufferNdx < m_vertexBuffers.size(); bufferNdx++)
	{
		VK_CHECK(vk.freeMemory(vkDevice, m_vertexBufferAllocs[bufferNdx]->getMemory()));
		VK_CHECK(vk.destroyBuffer(vkDevice, m_vertexBuffers[bufferNdx]));
	}

	for (size_t uniformNdx = 0; uniformNdx < m_uniformInfos.size(); uniformNdx++)
	{
		if (m_uniformInfos[uniformNdx].type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		{
			VK_CHECK(vk.destroyBufferView(vkDevice, m_uniformInfos[uniformNdx].descriptor.bufferView));
			VK_CHECK(vk.freeMemory(vkDevice, m_uniformInfos[uniformNdx].alloc->getMemory()));
			VK_CHECK(vk.destroyBuffer(vkDevice, m_uniformInfos[uniformNdx].buffer));
		}
		else if (m_uniformInfos[uniformNdx].type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
		{
			VK_CHECK(vk.destroyImageView(vkDevice, m_uniformInfos[uniformNdx].descriptor.imageView));
			VK_CHECK(vk.destroySampler(vkDevice, m_uniformInfos[uniformNdx].descriptor.sampler));
		}
		else
			DE_ASSERT(false);
	}
}

tcu::TestStatus ShaderRenderCaseInstance::iterate (void)
{
	setup();

	// Create quad grid.
	tcu::IVec2		viewportSize	= getViewportSize();
	int				width			= viewportSize.x();
	int 			height			= viewportSize.y();

	QuadGrid		quadGrid		(m_isVertexCase ? GRID_SIZE : 4, width, height, tcu::Vec4(0.125f, 0.25f, 0.5f, 1.0f), m_userAttribTransforms, m_textures);

	// Render result.
	tcu::Surface	resImage		(width, height);
	render(resImage, quadGrid);

	// Compute reference.
	tcu::Surface	refImage		(width, height);
	if (m_isVertexCase)
		computeVertexReference(refImage, quadGrid);
	else
		computeFragmentReference(refImage, quadGrid);

	// Compare.
	bool 			compareOk		= compareImages(resImage, refImage, 0.05f);

	if (compareOk)
		return tcu::TestStatus::pass("Result image matches reference");
	else
		return tcu::TestStatus::fail("Image mismatch");
}

void ShaderRenderCaseInstance::setupUniformData (deUint32 bindingLocation, deUint32 size, const void* dataPtr)
{
	const VkDevice					vkDevice			= m_context.getDevice();
	const DeviceInterface&			vk					= m_context.getDeviceInterface();
	const deUint32					queueFamilyIndex	= m_context.getUniversalQueueFamilyIndex();

	const VkBufferCreateInfo 		uniformBufferParams	=
	{
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,		// VkStructureType		sType;
		DE_NULL,									// const void*			pNext;
		size,										// VkDeviceSize			size;
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,			// VkBufferUsageFlags	usage;
		0u,											// VkBufferCreateFlags	flags;
		VK_SHARING_MODE_EXCLUSIVE,					// VkSharingMode		sharingMode;
		1u,											// deUint32				queueFamilyCount;
		&queueFamilyIndex							// const deUint32*		pQueueFamilyIndices;
	};

	Move<VkBuffer>					buffer				= createBuffer(vk, vkDevice, &uniformBufferParams);
	de::MovePtr<Allocation> 		alloc				= m_memAlloc.allocate(getBufferMemoryRequirements(vk, vkDevice, *buffer), MemoryRequirement::Any);
	VK_CHECK(vk.bindBufferMemory(vkDevice, *buffer, alloc->getMemory(), 0));

	void* bufferPtr;
	VK_CHECK(vk.mapMemory(vkDevice, alloc->getMemory(), 0, size, 0, &bufferPtr));
	deMemcpy(bufferPtr, dataPtr, size);
	VK_CHECK(vk.unmapMemory(vkDevice, alloc->getMemory()));

	const VkBufferViewCreateInfo	viewInfo 	=
	{
		VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO, 	// VkStructureType	sType;
		DE_NULL,									// void* 			pNext;
		*buffer,									// VkBuffer			buffer;
		VK_BUFFER_VIEW_TYPE_FORMATTED,				// VkBufferViewType	viewType;
		VK_FORMAT_R32_SFLOAT,						// VkFormat	format;
		0u,											// VkDeviceSize	offset;
		size										// VkDeviceSize	range;
	};

	Move<VkBufferView>				bufferView			= createBufferView(vk, vkDevice, &viewInfo);

	const VkDescriptorInfo 			descriptor			=
	{
		bufferView.disown(),						// VkBufferView		bufferView;
		0,											// VkSampler		sampler;
		0,											// VkImageView		imageView;
		0,											// VkAttachmentView	attachmentView;
		(vk::VkImageLayout)0,						// VkImageLayout	imageLayout;
	};

	UniformInfo uniformInfo;
	uniformInfo.buffer = buffer.disown();
	uniformInfo.alloc = alloc.release();
	uniformInfo.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformInfo.descriptor = descriptor;
	uniformInfo.location = bindingLocation;

	m_uniformInfos.push_back(uniformInfo);
}

void ShaderRenderCaseInstance::addUniform (deUint32 bindingLocation, vk::VkDescriptorType descriptorType, deUint32 dataSize, const void* data)
{
	m_descriptorSetLayoutBuilder.addSingleBinding(descriptorType, vk::VK_SHADER_STAGE_VERTEX_BIT | vk::VK_SHADER_STAGE_FRAGMENT_BIT);
	m_descriptorPoolBuilder.addType(descriptorType);

	setupUniformData(bindingLocation, dataSize, data);
}

void ShaderRenderCaseInstance::addAttribute (deUint32 bindingLocation, vk::VkFormat format, deUint32 sizePerElement, deUint32 count, const void* dataPtr)
{
	// Add binding specification
	const deUint32 							binding					= (deUint32)m_vertexBindingDescription.size();
	const VkVertexInputBindingDescription	bindingDescription		=
	{
		binding,
		sizePerElement,
		VK_VERTEX_INPUT_STEP_RATE_VERTEX
	};

	m_vertexBindingDescription.push_back(bindingDescription);

	// Add location and format specification
	const VkVertexInputAttributeDescription	attributeDescription	=
	{
		bindingLocation,			// deUint32	location;
		binding,					// deUint32	binding;
		format,						// VkFormat	format;
		0u,							// deUint32	offsetInBytes;
	};

	m_vertexattributeDescription.push_back(attributeDescription);

	// Upload data to buffer
	const VkDevice							vkDevice				= m_context.getDevice();
	const DeviceInterface&					vk						= m_context.getDeviceInterface();
	const deUint32							queueFamilyIndex		= m_context.getUniversalQueueFamilyIndex();

	const VkDeviceSize 						inputSize				= sizePerElement * count;
	const VkBufferCreateInfo				vertexBufferParams		=
	{
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,		// VkStructureType		sType;
		DE_NULL,									// const void*			pNext;
		inputSize,									// VkDeviceSize			size;
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,			// VkBufferUsageFlags	usage;
		0u,											// VkBufferCreateFlags	flags;
		VK_SHARING_MODE_EXCLUSIVE,					// VkSharingMode		sharingMode;
		1u,											// deUint32				queueFamilyCount;
		&queueFamilyIndex							// const deUint32*		pQueueFamilyIndices;
	};

	Move<VkBuffer> 							buffer					= createBuffer(vk, vkDevice, &vertexBufferParams);
	de::MovePtr<vk::Allocation>				alloc					= m_memAlloc.allocate(getBufferMemoryRequirements(vk, vkDevice, *buffer), MemoryRequirement::Any);

	VK_CHECK(vk.bindBufferMemory(vkDevice, *buffer, alloc->getMemory(), 0));

	void* bufferPtr;
	VK_CHECK(vk.mapMemory(vkDevice, alloc->getMemory(), 0, inputSize, 0, &bufferPtr));
	deMemcpy(bufferPtr, dataPtr, inputSize);
	VK_CHECK(vk.unmapMemory(vkDevice, alloc->getMemory()));

	m_vertexBuffers.push_back(buffer.disown());
	m_vertexBufferAllocs.push_back(alloc.release());
}

void ShaderRenderCaseInstance::useAttribute (deUint32 bindingLocation, BaseAttributeType type)
{
	const EnabledBaseAttribute attribute =
	{
		bindingLocation,
		type
	};
	m_enabledBaseAttributes.push_back(attribute);
}

void ShaderRenderCaseInstance::setup (void)
{
}

void ShaderRenderCaseInstance::setupUniforms (const tcu::Vec4& constCoords)
{
	m_uniformSetup.setup(*this, constCoords);
}

void ShaderRenderCaseInstance::useUniform (deUint32 bindingLocation, BaseUniformType type)
{
	#define UNIFORM_CASE(type, value) case type: addUniform(bindingLocation, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, value); break

	switch(type)
	{
		// Bool
		UNIFORM_CASE(UB_FALSE,	0);
		UNIFORM_CASE(UB_TRUE,	1);

		// BVec4
		UNIFORM_CASE(UB4_FALSE,	tcu::Vec4(0));
		UNIFORM_CASE(UB4_TRUE,	tcu::Vec4(1));

		// Integer
		UNIFORM_CASE(UI_ZERO, 	0);
		UNIFORM_CASE(UI_ONE, 	1);
		UNIFORM_CASE(UI_TWO, 	2);
		UNIFORM_CASE(UI_THREE, 	3);
		UNIFORM_CASE(UI_FOUR, 	4);
		UNIFORM_CASE(UI_FIVE, 	5);
		UNIFORM_CASE(UI_SIX, 	6);
		UNIFORM_CASE(UI_SEVEN, 	7);
		UNIFORM_CASE(UI_EIGHT, 	8);
		UNIFORM_CASE(UI_ONEHUNDREDONE, 101);

		// IVec2
		UNIFORM_CASE(UI2_MINUS_ONE, tcu::IVec2(-1));
		UNIFORM_CASE(UI2_ZERO,		tcu::IVec2(0));
		UNIFORM_CASE(UI2_ONE,		tcu::IVec2(1));
		UNIFORM_CASE(UI2_TWO,		tcu::IVec2(2));
		UNIFORM_CASE(UI2_THREE,		tcu::IVec2(3));
		UNIFORM_CASE(UI2_FOUR,		tcu::IVec2(4));
		UNIFORM_CASE(UI2_FIVE,		tcu::IVec2(5));

		// IVec3
		UNIFORM_CASE(UI3_MINUS_ONE,	tcu::IVec3(-1));
		UNIFORM_CASE(UI3_ZERO,		tcu::IVec3(0));
		UNIFORM_CASE(UI3_ONE,		tcu::IVec3(1));
		UNIFORM_CASE(UI3_TWO,		tcu::IVec3(2));
		UNIFORM_CASE(UI3_THREE,		tcu::IVec3(3));
		UNIFORM_CASE(UI3_FOUR,		tcu::IVec3(4));
		UNIFORM_CASE(UI3_FIVE,		tcu::IVec3(5));

		// IVec4
		UNIFORM_CASE(UI4_MINUS_ONE, tcu::IVec4(-1));
		UNIFORM_CASE(UI4_ZERO,		tcu::IVec4(0));
		UNIFORM_CASE(UI4_ONE,		tcu::IVec4(1));
		UNIFORM_CASE(UI4_TWO,		tcu::IVec4(2));
		UNIFORM_CASE(UI4_THREE,		tcu::IVec4(3));
		UNIFORM_CASE(UI4_FOUR,		tcu::IVec4(4));
		UNIFORM_CASE(UI4_FIVE,		tcu::IVec4(5));

		// Float
		UNIFORM_CASE(UF_ZERO,		0.0f);
		UNIFORM_CASE(UF_ONE,		1.0f);
		UNIFORM_CASE(UF_TWO,		2.0f);
		UNIFORM_CASE(UF_THREE,		3.0f);
		UNIFORM_CASE(UF_FOUR,		4.0f);
		UNIFORM_CASE(UF_FIVE,		5.0f);
		UNIFORM_CASE(UF_SIX,		6.0f);
		UNIFORM_CASE(UF_SEVEN,		7.0f);
		UNIFORM_CASE(UF_EIGHT,		8.0f);

		UNIFORM_CASE(UF_HALF,		1.0f / 2.0f);
		UNIFORM_CASE(UF_THIRD,		1.0f / 3.0f);
		UNIFORM_CASE(UF_FOURTH,		1.0f / 4.0f);
		UNIFORM_CASE(UF_FIFTH,		1.0f / 5.0f);
		UNIFORM_CASE(UF_SIXTH,		1.0f / 6.0f);
		UNIFORM_CASE(UF_SEVENTH,	1.0f / 7.0f);
		UNIFORM_CASE(UF_EIGHTH,		1.0f / 8.0f);

		// Vec2
		UNIFORM_CASE(UV2_MINUS_ONE,	tcu::Vec2(-1.0f));
		UNIFORM_CASE(UV2_ZERO,		tcu::Vec2(0.0f));
		UNIFORM_CASE(UV2_ONE,		tcu::Vec2(1.0f));
		UNIFORM_CASE(UV2_TWO,		tcu::Vec2(2.0f));
		UNIFORM_CASE(UV2_THREE,		tcu::Vec2(3.0f));

		UNIFORM_CASE(UV2_HALF,		tcu::Vec2(1.0f / 2.0f));

		// Vec3
		UNIFORM_CASE(UV3_MINUS_ONE,	tcu::Vec3(-1.0f));
		UNIFORM_CASE(UV3_ZERO,		tcu::Vec3(0.0f));
		UNIFORM_CASE(UV3_ONE,		tcu::Vec3(1.0f));
		UNIFORM_CASE(UV3_TWO,		tcu::Vec3(2.0f));
		UNIFORM_CASE(UV3_THREE,		tcu::Vec3(3.0f));

		UNIFORM_CASE(UV3_HALF,		tcu::Vec3(1.0f / 2.0f));

		// Vec4
		UNIFORM_CASE(UV4_MINUS_ONE,	tcu::Vec4(-1.0f));
		UNIFORM_CASE(UV4_ZERO,		tcu::Vec4(0.0f));
		UNIFORM_CASE(UV4_ONE,		tcu::Vec4(1.0f));
		UNIFORM_CASE(UV4_TWO,		tcu::Vec4(2.0f));
		UNIFORM_CASE(UV4_THREE,		tcu::Vec4(3.0f));

		UNIFORM_CASE(UV4_HALF,		tcu::Vec4(1.0f / 2.0f));

		UNIFORM_CASE(UV4_BLACK,		tcu::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		UNIFORM_CASE(UV4_GRAY,		tcu::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
		UNIFORM_CASE(UV4_WHITE,		tcu::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

		default:
			m_context.getTestContext().getLog() << tcu::TestLog::Message << "Unknown Uniform type: " << type << tcu::TestLog::EndMessage;
			break;
	}

	#undef UNIFORM_CASE
}

tcu::IVec2 ShaderRenderCaseInstance::getViewportSize (void) const
{
	return tcu::IVec2(de::min(m_renderSize.x(), MAX_RENDER_WIDTH),
					  de::min(m_renderSize.y(), MAX_RENDER_HEIGHT));
}

void ShaderRenderCaseInstance::useSampler2D (deUint32 bindingLocation, deUint32 textureID)
{
	const VkDevice				vkDevice			= m_context.getDevice();
	const DeviceInterface&		vk					= m_context.getDeviceInterface();

	DE_ASSERT(textureID < m_textures.size());

	const TextureBinding&		textureBinding		= m_textures[textureID];
	const Texture2D*			texture				= textureBinding.get2D();
	const tcu::Sampler&			refSampler			= textureBinding.getSampler();

	// Create sampler
	const VkSamplerCreateInfo	samplerParams		=
	{
		VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		DE_NULL,
		mapTexFilter(refSampler.magFilter),
		mapTexFilter(refSampler.minFilter),
		mapTexMipmapMode(refSampler.minFilter),
		mapWrapMode(refSampler.wrapS),
		mapWrapMode(refSampler.wrapT),
		mapWrapMode(refSampler.wrapR),
		refSampler.lodThreshold,
		1,
		(refSampler.compare != tcu::Sampler::COMPAREMODE_NONE),
		mapCompareMode(refSampler.compare),
		0.0f,
		0.0f,
		VK_BORDER_COLOR_INT_OPAQUE_WHITE
	};

	Move<VkSampler>				sampler				= createSampler(vk, vkDevice, &samplerParams);

	const VkImageViewCreateInfo	viewParams			=
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = NULL,
		.image = *texture->getVkTexture(),
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = texture->getVkFormat(),
		.channels = { VK_CHANNEL_SWIZZLE_R,
						VK_CHANNEL_SWIZZLE_G,
						VK_CHANNEL_SWIZZLE_B,
						VK_CHANNEL_SWIZZLE_A },
		.subresourceRange = { VK_IMAGE_ASPECT_COLOR, 0, 1, 0, 1 },
	};

	Move<VkImageView>			imageView			= createImageView(vk, vkDevice, &viewParams);

	const vk::VkDescriptorInfo	descriptor			=
	{
		0,											// VkBufferView		bufferView;
		sampler.disown(),							// VkSampler		sampler;
		imageView.disown(),							// VkImageView		imageView;
		0,											// VkAttachmentView	attachmentView;
		vk::VK_IMAGE_LAYOUT_GENERAL,				// VkImageLayout	imageLayout;
	};

	UniformInfo newUniformInfo;
	m_uniformInfos.push_back(newUniformInfo);

	UniformInfo&				uniformInfo			= m_uniformInfos[m_uniformInfos.size() - 1];

	uniformInfo.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	uniformInfo.descriptor = descriptor;
	uniformInfo.location = bindingLocation;

	m_descriptorSetLayoutBuilder.addSingleSamplerBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, vk::VK_SHADER_STAGE_FRAGMENT_BIT, &uniformInfo.descriptor.sampler);
	m_descriptorPoolBuilder.addType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
}

void ShaderRenderCaseInstance::setupDefaultInputs (const QuadGrid& quadGrid)
{
	/* Configuration of the vertex input attributes:
		a_position   is at location 0
		a_coords     is at location 1
		a_unitCoords is at location 2
		a_one        is at location 3

	  User attributes starts from at the location 4.
	*/
	addAttribute(0u, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(tcu::Vec4), quadGrid.getNumVertices(), quadGrid.getPositions());
	addAttribute(1u, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(tcu::Vec4), quadGrid.getNumVertices(), quadGrid.getCoords());
	addAttribute(2u, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(tcu::Vec4), quadGrid.getNumVertices(), quadGrid.getUnitCoords());
	addAttribute(3u, VK_FORMAT_R32_SFLOAT, sizeof(float), quadGrid.getNumVertices(), quadGrid.getAttribOne());

	static const struct
	{
		BaseAttributeType	type;
		int					userNdx;
	} userAttributes[] =
	{
		{ A_IN0, 0 },
		{ A_IN1, 1 },
		{ A_IN2, 2 },
		{ A_IN3, 3 }
	};

	static const struct
	{
		BaseAttributeType	matrixType;
		int					numCols;
		int					numRows;
	} matrices[] =
	{
		{ MAT2,		2, 2 },
		{ MAT2x3,	2, 3 },
		{ MAT2x4,	2, 4 },
		{ MAT3x2,	3, 2 },
		{ MAT3,		3, 3 },
		{ MAT3x4,	3, 4 },
		{ MAT4x2,	4, 2 },
		{ MAT4x3,	4, 3 },
		{ MAT4,		4, 4 }
	};

	for (size_t i = 0; i < m_enabledBaseAttributes.size(); i++)
	{
		for (int userNdx = 0; userNdx < DE_LENGTH_OF_ARRAY(userAttributes); userNdx++)
		{
			if (userAttributes[userNdx].type != m_enabledBaseAttributes[i].type)
				continue;

			addAttribute(m_enabledBaseAttributes[i].location, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(tcu::Vec4), quadGrid.getNumVertices(), quadGrid.getUserAttrib(userNdx));
		}

		for (int matNdx = 0; matNdx < DE_LENGTH_OF_ARRAY(matrices); matNdx++)
		{

			if (matrices[matNdx].matrixType != m_enabledBaseAttributes[i].type)
				continue;

			int numCols = matrices[matNdx].numCols;

			for (int colNdx = 0; colNdx < numCols; colNdx++)
			{
				addAttribute(m_enabledBaseAttributes[i].location + colNdx, VK_FORMAT_R32G32B32A32_SFLOAT, 4 * sizeof(float), quadGrid.getNumVertices(), quadGrid.getUserAttrib(colNdx));
			}
		}
	}
}

void ShaderRenderCaseInstance::render (tcu::Surface& result, const QuadGrid& quadGrid)
{
	const VkDevice				vkDevice			= m_context.getDevice();
	const DeviceInterface&		vk					= m_context.getDeviceInterface();
	const VkQueue				queue				= m_context.getUniversalQueue();
	const deUint32				queueFamilyIndex	= m_context.getUniversalQueueFamilyIndex();

	// Create color image
	{
		const VkImageCreateInfo colorImageParams =
		{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,										// VkStructureType		sType;
			DE_NULL,																	// const void*			pNext;
			VK_IMAGE_TYPE_2D,															// VkImageType			imageType;
			m_colorFormat,																// VkFormat				format;
			{ m_renderSize.x(), m_renderSize.y(), 1u },									// VkExtent3D			extent;
			1u,																			// deUint32				mipLevels;
			1u,																			// deUint32				arraySize;
			1u,																			// deUint32				samples;
			VK_IMAGE_TILING_OPTIMAL,													// VkImageTiling		tiling;
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SOURCE_BIT,	// VkImageUsageFlags	usage;
			0u,																			// VkImageCreateFlags	flags;
			VK_SHARING_MODE_EXCLUSIVE,													// VkSharingMode		sharingMode;
			1u,																			// deUint32				queueFamilyCount;
			&queueFamilyIndex															// const deUint32*		pQueueFamilyIndices;
		};

		m_colorImage = createImage(vk, vkDevice, &colorImageParams);

		// Allocate and bind color image memory
		m_colorImageAlloc = m_memAlloc.allocate(getImageMemoryRequirements(vk, vkDevice, *m_colorImage), MemoryRequirement::HostVisible);
		VK_CHECK(vk.bindImageMemory(vkDevice, *m_colorImage, m_colorImageAlloc->getMemory(), 0));
	}

	// Create color attachment view
	{
		const VkAttachmentViewCreateInfo colorAttachmentViewParams =
		{
			VK_STRUCTURE_TYPE_ATTACHMENT_VIEW_CREATE_INFO,		// VkStructureType				sType;
			DE_NULL,											// constvoid*					pNext;
			*m_colorImage,										// VkImage						image;
			m_colorFormat,										// VkFormat						format;
			0u,													// deUint32						mipLevel;
			0u,													// deUint32						baseArraySlice;
			1u,													// deUint32						arraySize;
			0u													// VkAttachmentViewCreateFlags	flags;
		};

		m_colorAttachmentView = createAttachmentView(vk, vkDevice, &colorAttachmentViewParams);
	}

	// Create render pass
	{
		const VkAttachmentDescription colorAttachmentDescription =
		{
			VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION,			// VkStructureType		sType;
			DE_NULL,											// const void*			pNext;
			m_colorFormat,										// VkFormat				format;
			1u,													// deUint32				samples;
			VK_ATTACHMENT_LOAD_OP_CLEAR,						// VkAttachmentLoadOp	loadOp;
			VK_ATTACHMENT_STORE_OP_STORE,						// VkAttachmentStoreOp	storeOp;
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,					// VkAttachmentLoadOp	stencilLoadOp;
			VK_ATTACHMENT_STORE_OP_DONT_CARE,					// VkAttachmentStoreOp	stencilStoreOp;
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,			// VkImageLayout		initialLayout;
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL			// VkImageLayout		finalLayout;
		};

		const VkAttachmentDescription attachments[1] =
		{
			colorAttachmentDescription
		};

		const VkAttachmentReference colorAttachmentReference =
		{
			0u,													// deUint32			attachment;
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL			// VkImageLayout	layout;
		};

		const VkSubpassDescription subpassDescription =
		{
			VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION,				// VkStructureType				sType;
			DE_NULL,											// constvoid*					pNext;
			VK_PIPELINE_BIND_POINT_GRAPHICS,					// VkPipelineBindPoint			pipelineBindPoint;
			0u,													// VkSubpassDescriptionFlags	flags;
			0u,													// deUint32						inputCount;
			DE_NULL,											// constVkAttachmentReference*	inputAttachments;
			1u,													// deUint32						colorCount;
			&colorAttachmentReference,							// constVkAttachmentReference*	colorAttachments;
			DE_NULL,											// constVkAttachmentReference*	resolveAttachments;
			{ ~0u, VK_IMAGE_LAYOUT_GENERAL },					// VkAttachmentReference		depthStencilAttachment;
			0u,													// deUint32						preserveCount;
			DE_NULL												// constVkAttachmentReference*	preserveAttachments;
		};

		const VkRenderPassCreateInfo renderPassParams =
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,			// VkStructureType					sType;
			DE_NULL,											// const void*						pNext;
			1u,													// deUint32							attachmentCount;
			attachments,										// const VkAttachmentDescription*	pAttachments;
			1u,													// deUint32							subpassCount;
			&subpassDescription,								// const VkSubpassDescription*		pSubpasses;
			0u,													// deUint32							dependencyCount;
			DE_NULL												// const VkSubpassDependency*		pDependencies;
		};

		m_renderPass = createRenderPass(vk, vkDevice, &renderPassParams);
	}

	// Create framebuffer
	{
		const VkAttachmentBindInfo attachmentBindInfos[1] =
		{
			{ *m_colorAttachmentView, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL  },
		};

		const VkFramebufferCreateInfo framebufferParams =
		{
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,			// VkStructureType				sType;
			DE_NULL,											// const void*					pNext;
			*m_renderPass,										// VkRenderPass					renderPass;
			1u,													// deUint32						attachmentCount;
			attachmentBindInfos,								// const VkAttachmentBindInfo*	pAttachments;
			(deUint32)m_renderSize.x(),							// deUint32						width;
			(deUint32)m_renderSize.y(),							// deUint32						height;
			1u													// deUint32						layers;
		};

		m_framebuffer = createFramebuffer(vk, vkDevice, &framebufferParams);
	}

	// Create descriptors
	{
		setupUniforms(quadGrid.getConstCoords());

		m_descriptorSetLayout = m_descriptorSetLayoutBuilder.build(vk, vkDevice);
		m_descriptorPool = m_descriptorPoolBuilder.build(vk, vkDevice, VK_DESCRIPTOR_POOL_USAGE_ONE_SHOT, 1u);
		m_descriptorSet = allocDescriptorSet(vk, vkDevice, *m_descriptorPool, VK_DESCRIPTOR_SET_USAGE_STATIC, *m_descriptorSetLayout);

		for(deUint32 i = 0; i < m_uniformInfos.size(); i++)
		{
			deUint32 location = m_uniformInfos[i].location;
			m_descriptorSetUpdateBuilder.writeSingle(*m_descriptorSet, DescriptorSetUpdateBuilder::Location::binding(location), m_uniformInfos[i].type, &m_uniformInfos[i].descriptor);
		}

		m_descriptorSetUpdateBuilder.update(vk, vkDevice);
	}

	// Create pipeline layout
	{
		const VkPipelineLayoutCreateInfo pipelineLayoutParams =
		{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,		// VkStructureType				sType;
			DE_NULL,											// const void*					pNext;
			1u,													// deUint32						descriptorSetCount;
			&*m_descriptorSetLayout,							// const VkDescriptorSetLayout*	pSetLayouts;
			0u,													// deUint32						pushConstantRangeCount;
			DE_NULL												// const VkPushConstantRange*	pPushConstantRanges;
		};

		m_pipelineLayout = createPipelineLayout(vk, vkDevice, &pipelineLayoutParams);
	}

	// Create shaders
	{
		m_vertexShaderModule	= createShaderModule(vk, vkDevice, m_context.getBinaryCollection().get("vert"), 0);
		m_fragmentShaderModule	= createShaderModule(vk, vkDevice, m_context.getBinaryCollection().get("frag"), 0);

		const VkShaderCreateInfo vertexShaderParams =
		{
			VK_STRUCTURE_TYPE_SHADER_CREATE_INFO,			// VkStructureType		sType;
			DE_NULL,										// const void*			pNext;
			*m_vertexShaderModule,							// VkShaderModule		module;
			"main",											// const char*			pName;
			0u												// VkShaderCreateFlags	flags;
		};

		const VkShaderCreateInfo fragmentShaderParams =
		{
			VK_STRUCTURE_TYPE_SHADER_CREATE_INFO,			// VkStructureType		sType;
			DE_NULL,										// const void*			pNext;
			*m_fragmentShaderModule,						// VkShaderModule		module;
			"main",											// const char*			pName;
			0u												// VkShaderCreateFlags	flags;
		};

		m_vertexShader	= createShader(vk, vkDevice, &vertexShaderParams);
		m_fragmentShader= createShader(vk, vkDevice, &fragmentShaderParams);
	}

	// Create pipeline
	{
		const VkPipelineShaderStageCreateInfo shaderStageParams[2] =
		{
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,		// VkStructureType				sType;
				DE_NULL,													// const void*					pNext;
				VK_SHADER_STAGE_VERTEX,										// VkShaderStage				stage;
				*m_vertexShader,											// VkShader						shader;
				DE_NULL														// const VkSpecializationInfo*	pSpecializationInfo;
			},
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,		// VkStructureType				sType;
				DE_NULL,													// const void*					pNext;
				VK_SHADER_STAGE_FRAGMENT,									// VkShaderStage				stage;
				*m_fragmentShader,											// VkShader						shader;
				DE_NULL														// const VkSpecializationInfo*	pSpecializationInfo;
			}
		};

		// Add base attributes

		// Add test case specific attributes
		if (m_attribFunc)
			m_attribFunc(*this, quadGrid.getNumVertices());

		setupDefaultInputs(quadGrid);

		const VkPipelineVertexInputStateCreateInfo vertexInputStateParams =
		{
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,		// VkStructureType							sType;
			DE_NULL,														// const void*								pNext;
			(deUint32)m_vertexBindingDescription.size(),					// deUint32									bindingCount;
			&m_vertexBindingDescription[0],									// const VkVertexInputBindingDescription*	pVertexBindingDescriptions;
			(deUint32)m_vertexattributeDescription.size(),					// deUint32									attributeCount;
			&m_vertexattributeDescription[0],								// const VkVertexInputAttributeDescription*	pVertexAttributeDescriptions;
		};

		const VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateParams =
		{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,	// VkStructureType		sType;
			DE_NULL,														// const void*			pNext;
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,							// VkPrimitiveTopology	topology;
			false															// VkBool32				primitiveRestartEnable;
		};

		const VkPipelineViewportStateCreateInfo viewportStateParams =
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,			// VkStructureType	sType;
			DE_NULL,														// const void*		pNext;
			1u																// deUint32			viewportCount;
		};

		const VkPipelineRasterStateCreateInfo rasterStateParams =
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTER_STATE_CREATE_INFO,			// VkStructureType	sType;
			DE_NULL,														// const void*		pNext;
			false,															// VkBool32			depthClipEnable;
			false,															// VkBool32			rasterizerDiscardEnable;
			VK_FILL_MODE_SOLID,												// VkFillMode		fillMode;
			VK_CULL_MODE_NONE,												// VkCullMode		cullMode;
			VK_FRONT_FACE_CCW												// VkFrontFace		frontFace;
		};

		const VkPipelineColorBlendAttachmentState colorBlendAttachmentState =
		{
			false,																		// VkBool32			blendEnable;
			VK_BLEND_ONE,																// VkBlend			srcBlendColor;
			VK_BLEND_ZERO,																// VkBlend			destBlendColor;
			VK_BLEND_OP_ADD,															// VkBlendOp		blendOpColor;
			VK_BLEND_ONE,																// VkBlend			srcBlendAlpha;
			VK_BLEND_ZERO,																// VkBlend			destBlendAlpha;
			VK_BLEND_OP_ADD,															// VkBlendOp		blendOpAlpha;
			VK_CHANNEL_R_BIT | VK_CHANNEL_G_BIT | VK_CHANNEL_B_BIT | VK_CHANNEL_A_BIT	// VkChannelFlags	channelWriteMask;
		};

		const VkPipelineColorBlendStateCreateInfo colorBlendStateParams =
		{
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,	// VkStructureType								sType;
			DE_NULL,													// const void*									pNext;
			false,														// VkBool32										alphaToCoverageEnable;
			false,														// VkBool32										logicOpEnable;
			VK_LOGIC_OP_COPY,											// VkLogicOp									logicOp;
			1u,															// deUint32										attachmentCount;
			&colorBlendAttachmentState									// const VkPipelineColorBlendAttachmentState*	pAttachments;
		};

		const VkGraphicsPipelineCreateInfo graphicsPipelineParams =
		{
			VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,	// VkStructureType									sType;
			DE_NULL,											// const void*										pNext;
			2u,													// deUint32											stageCount;
			shaderStageParams,									// const VkPipelineShaderStageCreateInfo*			pStages;
			&vertexInputStateParams,							// const VkPipelineVertexInputStateCreateInfo*		pVertexInputState;
			&inputAssemblyStateParams,							// const VkPipelineInputAssemblyStateCreateInfo*	pInputAssemblyState;
			DE_NULL,											// const VkPipelineTessellationStateCreateInfo*		pTessellationState;
			&viewportStateParams,								// const VkPipelineViewportStateCreateInfo*			pViewportState;
			&rasterStateParams,									// const VkPipelineRasterStateCreateInfo*			pRasterState;
			DE_NULL,											// const VkPipelineMultisampleStateCreateInfo*		pMultisampleState;
			DE_NULL,											// const VkPipelineDepthStencilStateCreateInfo*		pDepthStencilState;
			&colorBlendStateParams,								// const VkPipelineColorBlendStateCreateInfo*		pColorBlendState;
			0u,													// VkPipelineCreateFlags							flags;
			*m_pipelineLayout,									// VkPipelineLayout									layout;
			*m_renderPass,										// VkRenderPass										renderPass;
			0u,													// deUint32											subpass;
			0u,													// VkPipeline										basePipelineHandle;
			0u													// deInt32											basePipelineIndex;
		};

		m_graphicsPipeline = createGraphicsPipeline(vk, vkDevice, DE_NULL, &graphicsPipelineParams);
	}

	// Create dynamic states
	{
		const VkViewport viewport =
		{
			0.0f,						// float	originX;
			0.0f,						// float	originY;
			(float)m_renderSize.x(),	// float	width;
			(float)m_renderSize.y(),	// float	height;
			0.0f,						// float	minDepth;
			1.0f						// float	maxDepth;
		};

		const VkRect2D scissor =
		{
			0,
			0,
			m_renderSize.x(),
			m_renderSize.y()
		};

		const VkDynamicViewportStateCreateInfo viewportStateParams =
		{
			VK_STRUCTURE_TYPE_DYNAMIC_VIEWPORT_STATE_CREATE_INFO,	// VkStructureType		sType;
			DE_NULL,												// const void*			pNext;
			1,														// deUint32				viewportAndScissorCount;
			&viewport,												// const VkViewport*	pViewports;
			&scissor												// const VkRect2D*		pScissors;
		};

		const VkDynamicRasterStateCreateInfo rasterStateParams =
		{
			VK_STRUCTURE_TYPE_DYNAMIC_RASTER_STATE_CREATE_INFO,		// VkStructureType	sType;
			DE_NULL,												// const void*		pNext;
			0.0f,													// float			depthBias;
			0.0f,													// float			depthBiasClamp;
			0.0f,													// float			slopeScaledDepthBias;
			1.0f,													// float			lineWidth;
		};


		const VkDynamicColorBlendStateCreateInfo colorBlendStateParams =
		{
			VK_STRUCTURE_TYPE_DYNAMIC_COLOR_BLEND_STATE_CREATE_INFO,	// VkStructureType	sType;
			DE_NULL,													// const void*		pNext;
			{ 0.0f, 0.0f, 0.0f, 0.0f }									// float			blendConst[4];
		};

		m_viewportState		= createDynamicViewportState(vk, vkDevice, &viewportStateParams);
		m_rasterState		= createDynamicRasterState(vk, vkDevice, &rasterStateParams);
		m_colorBlendState	= createDynamicColorBlendState(vk, vkDevice, &colorBlendStateParams);
	}

	// Create vertex indices buffer
	{
		const VkDeviceSize indiceBufferSize = quadGrid.getNumTriangles() * 3 * sizeof(deUint16);
		const VkBufferCreateInfo indiceBufferParams =
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,		// VkStructureType		sType;
			DE_NULL,									// const void*			pNext;
			indiceBufferSize,							// VkDeviceSize			size;
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,			// VkBufferUsageFlags	usage;
			0u,											// VkBufferCreateFlags	flags;
			VK_SHARING_MODE_EXCLUSIVE,					// VkSharingMode		sharingMode;
			1u,											// deUint32				queueFamilyCount;
			&queueFamilyIndex							// const deUint32*		pQueueFamilyIndices;
		};

		m_indiceBuffer		= createBuffer(vk, vkDevice, &indiceBufferParams);
		m_indiceBufferAlloc	= m_memAlloc.allocate(getBufferMemoryRequirements(vk, vkDevice, *m_indiceBuffer), MemoryRequirement::HostVisible);

		VK_CHECK(vk.bindBufferMemory(vkDevice, *m_indiceBuffer, m_indiceBufferAlloc->getMemory(), 0));

		// Load vertice indices into buffer
		void* bufferPtr;
		VK_CHECK(vk.mapMemory(vkDevice, m_indiceBufferAlloc->getMemory(), 0, indiceBufferSize, 0, &bufferPtr));
		deMemcpy(bufferPtr, quadGrid.getIndices(), indiceBufferSize);
		VK_CHECK(vk.unmapMemory(vkDevice, m_indiceBufferAlloc->getMemory()));
	}

	// Create command pool
	{
		const VkCmdPoolCreateInfo cmdPoolParams =
		{
			VK_STRUCTURE_TYPE_CMD_POOL_CREATE_INFO,		// VkStructureType		sType;
			DE_NULL,									// const void*			pNext;
			queueFamilyIndex,							// deUint32				queueFamilyIndex;
			VK_CMD_POOL_CREATE_TRANSIENT_BIT			// VkCmdPoolCreateFlags	flags;
		};

		m_cmdPool = createCommandPool(vk, vkDevice, &cmdPoolParams);
	}

	// Create command buffer
	{
		const VkCmdBufferCreateInfo cmdBufferParams =
		{
			VK_STRUCTURE_TYPE_CMD_BUFFER_CREATE_INFO,	// VkStructureType			sType;
			DE_NULL,									// const void*				pNext;
			*m_cmdPool,									// VkCmdPool				cmdPool;
			VK_CMD_BUFFER_LEVEL_PRIMARY,				// VkCmdBufferLevel			level;
			0u											// VkCmdBufferCreateFlags	flags;
		};

		const VkCmdBufferBeginInfo cmdBufferBeginInfo =
		{
			VK_STRUCTURE_TYPE_CMD_BUFFER_BEGIN_INFO,	// VkStructureType			sType;
			DE_NULL,									// const void*				pNext;
			0u,											// VkCmdBufferOptimizeFlags	flags;
			DE_NULL,									// VkRenderPass				renderPass;
			DE_NULL										// VkFramebuffer			framebuffer;
		};

		const VkClearValue attachmentClearValues[1] =
		{
			{ m_clearColor.x(), m_clearColor.y(), m_clearColor.z(), m_clearColor.w() }
		};

		const VkRenderPassBeginInfo renderPassBeginInfo =
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO	,	// VkStructureType		sType;
			DE_NULL,										// const void*			pNext;
			*m_renderPass,									// VkRenderPass			renderPass;
			*m_framebuffer,									// VkFramebuffer		framebuffer;
			{ 0, 0, m_renderSize.x(), m_renderSize.y() },	// VkRect2D				renderArea;
			1,												// deUint32				attachmentCount;
			attachmentClearValues							// const VkClearValue*	pAttachmentClearValues;
		};

		m_cmdBuffer = createCommandBuffer(vk, vkDevice, &cmdBufferParams);

		VK_CHECK(vk.beginCommandBuffer(*m_cmdBuffer, &cmdBufferBeginInfo));

		// Add texture barriers
		std::vector<VkImageMemoryBarrier> barriers;
		std::vector<void*> barrierPtrs;

		for(size_t i = 0; i < m_textures.size(); i++)
		{
			const TextureBinding& textureBinding = m_textures[i];
			const Texture2D* texture = textureBinding.get2D();
			VkImageMemoryBarrier textureBarrier =
			{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				DE_NULL,
				VK_MEMORY_OUTPUT_HOST_WRITE_BIT | VK_MEMORY_OUTPUT_TRANSFER_BIT,
				0,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				queueFamilyIndex,
				queueFamilyIndex,
				*texture->getVkTexture(),
				{
					VK_IMAGE_ASPECT_COLOR,
					0,
					1,
					0,
					0
				}
			};

			barriers.push_back(textureBarrier);
			barrierPtrs.push_back((void*)&barriers[i]);
		}

		vk.cmdPipelineBarrier(*m_cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, false, (deUint32)barrierPtrs.size(), (const void * const*)&barrierPtrs[0]);

		vk.cmdBeginRenderPass(*m_cmdBuffer, &renderPassBeginInfo, VK_RENDER_PASS_CONTENTS_INLINE);

		vk.cmdBindDynamicViewportState(*m_cmdBuffer, *m_viewportState);
		vk.cmdBindDynamicRasterState(*m_cmdBuffer, *m_rasterState);

		vk.cmdBindPipeline(*m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_graphicsPipeline);
		vk.cmdBindDescriptorSets(*m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipelineLayout, 0u, 1, &*m_descriptorSet, 0u, DE_NULL);
		vk.cmdBindIndexBuffer(*m_cmdBuffer, *m_indiceBuffer, 0, VK_INDEX_TYPE_UINT16);


		const deUint32 numberOfVertexAttributes = (deUint32)m_vertexBuffers.size();
		std::vector<VkDeviceSize> offsets(numberOfVertexAttributes, 0);

		vk.cmdBindVertexBuffers(*m_cmdBuffer, 0, numberOfVertexAttributes, &m_vertexBuffers[0], &offsets[0]);
		vk.cmdDrawIndexed(*m_cmdBuffer, 0, quadGrid.getNumTriangles() * 3, 0, 0, 1);

		vk.cmdEndRenderPass(*m_cmdBuffer);
		VK_CHECK(vk.endCommandBuffer(*m_cmdBuffer));
	}

	// Create fence
	{
		const VkFenceCreateInfo fenceParams =
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,	// VkStructureType		sType;
			DE_NULL,								// const void*			pNext;
			0u										// VkFenceCreateFlags	flags;
		};
		m_fence = createFence(vk, vkDevice, &fenceParams);
	}

	// Execute Draw
	{
		VK_CHECK(vk.resetFences(vkDevice, 1, &m_fence.get()));
		VK_CHECK(vk.queueSubmit(queue, 1, &m_cmdBuffer.get(), *m_fence));
		VK_CHECK(vk.waitForFences(vkDevice, 1, &m_fence.get(), true, ~(0ull) /* infinity*/));
	}

	// Read back the result
	{
		const VkDeviceSize imageSizeBytes = (VkDeviceSize)(sizeof(deUint32) * m_renderSize.x() * m_renderSize.y());
		const VkBufferCreateInfo readImageBufferParams   =
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,		//  VkStructureType		sType;
			DE_NULL,									//  const void*			pNext;
			imageSizeBytes,								//  VkDeviceSize		size;
			VK_BUFFER_USAGE_TRANSFER_DESTINATION_BIT,	//  VkBufferUsageFlags	usage;
			0u,											//  VkBufferCreateFlags	flags;
			VK_SHARING_MODE_EXCLUSIVE,					//  VkSharingMode		sharingMode;
			1u,											//  deUint32			queueFamilyCount;
			&queueFamilyIndex,							//  const deUint32*		pQueueFamilyIndices;
		};
		const Unique<VkBuffer> readImageBuffer(createBuffer(vk, vkDevice, &readImageBufferParams));
		const de::UniquePtr<Allocation>	readImageBufferMemory(m_memAlloc.allocate(getBufferMemoryRequirements(vk, vkDevice, *readImageBuffer), MemoryRequirement::HostVisible));

		VK_CHECK(vk.bindBufferMemory(vkDevice, *readImageBuffer, readImageBufferMemory->getMemory(), readImageBufferMemory->getOffset()));


		// Copy image to buffer
		Move<VkCmdBuffer> cmdBuffer;

		const VkCmdBufferCreateInfo cmdBufferParams =
		{
			VK_STRUCTURE_TYPE_CMD_BUFFER_CREATE_INFO,	// VkStructureType			sType;
			DE_NULL,									// const void*				pNext;
			*m_cmdPool,									// VkCmdPool				cmdPool;
			VK_CMD_BUFFER_LEVEL_PRIMARY,				// VkCmdBufferLevel			level;
			0u											// VkCmdBufferCreateFlags	flags;
		};

		const VkCmdBufferBeginInfo cmdBufferBeginInfo =
		{
			VK_STRUCTURE_TYPE_CMD_BUFFER_BEGIN_INFO,	// VkStructureType			sType;
			DE_NULL,									// const void*				pNext;
			0u,											// VkCmdBufferOptimizeFlags	flags;
			DE_NULL,									// VkRenderPass				renderPass;
			DE_NULL										// VkFramebuffer			framebuffer;
		};

		cmdBuffer = createCommandBuffer(vk, vkDevice, &cmdBufferParams);

		const VkBufferImageCopy copyParams =
		{
			0u,											// VkDeviceSize			bufferOffset;
			m_renderSize.x() * 4u,						// deUint32				bufferRowLength;
			0u,											// deUint32				bufferImageHeight;
			{
				VK_IMAGE_ASPECT_COLOR,					// VkImageAspect		aspect;
				0u,										// deUint32				mipLevel;
				0u,										// deUint32				arraySlice;
			},											// VkImageSubresource	imageSubresource;
			{ 0u, 0u, 0u },								// VkOffset3D			imageOffset;
			{ m_renderSize.x(), m_renderSize.y(), 1u }	// VkExtent3D			imageExtent;
		};

		VK_CHECK(vk.beginCommandBuffer(*cmdBuffer, &cmdBufferBeginInfo));
		vk.cmdCopyImageToBuffer(*cmdBuffer, *m_colorImage, VK_IMAGE_LAYOUT_TRANSFER_SOURCE_OPTIMAL, *readImageBuffer, 1u, &copyParams);
		VK_CHECK(vk.endCommandBuffer(*cmdBuffer));

		VK_CHECK(vk.resetFences(vkDevice, 1, &m_fence.get()));
		VK_CHECK(vk.queueSubmit(queue, 1, &cmdBuffer.get(), *m_fence));
		VK_CHECK(vk.waitForFences(vkDevice, 1, &m_fence.get(), true, ~(0ull) /* infinity */));

		void *imagePtr;
		VK_CHECK(vk.mapMemory(vkDevice, readImageBufferMemory->getMemory(), readImageBufferMemory->getOffset(), imageSizeBytes, 0u, &imagePtr));

		const VkMappedMemoryRange range =
		{
			VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,		// VkStructureType		sType;
			DE_NULL,									// const void*			pNext;
			readImageBufferMemory->getMemory(),			// VkDeviceMemory		mem;
			0,											// VkDeviceSize			offset;
			imageSizeBytes,								// VkDeviceSize			size;
		};

		VK_CHECK(vk.invalidateMappedMemoryRanges(vkDevice, 1u, &range));

		tcu::TextureFormat resultFormat(tcu::TextureFormat::RGBA, tcu::TextureFormat::UNORM_INT8);
		const tcu::ConstPixelBufferAccess resultAccess(resultFormat, m_renderSize.x(), m_renderSize.y(), 1, imagePtr);

		tcu::copy(result.getAccess(), resultAccess);

		VK_CHECK(vk.unmapMemory(vkDevice, readImageBufferMemory->getMemory()));
	}
}

void ShaderRenderCaseInstance::computeVertexReference (tcu::Surface& result, const QuadGrid& quadGrid)
{
	// Buffer info.
	int						width		= result.getWidth();
	int						height		= result.getHeight();
	int						gridSize	= quadGrid.getGridSize();
	int						stride		= gridSize + 1;
	bool					hasAlpha	= true; // \todo [2015-09-07 elecro] add correct alpha check
	ShaderEvalContext		evalCtx		(quadGrid);

	// Evaluate color for each vertex.
	std::vector<tcu::Vec4>	colors		((gridSize + 1) * (gridSize + 1));
	for (int y = 0; y < gridSize+1; y++)
	for (int x = 0; x < gridSize+1; x++)
	{
		float		sx			= (float)x / (float)gridSize;
		float		sy			= (float)y / (float)gridSize;
		int			vtxNdx		= ((y * (gridSize+1)) + x);

		evalCtx.reset(sx, sy);
		m_evaluator.evaluate(evalCtx);
		DE_ASSERT(!evalCtx.isDiscarded); // Discard is not available in vertex shader.
		tcu::Vec4 color = evalCtx.color;

		if (!hasAlpha)
			color.w() = 1.0f;

		colors[vtxNdx] = color;
	}

	// Render quads.
	for (int y = 0; y < gridSize; y++)
	for (int x = 0; x < gridSize; x++)
	{
		float		x0		= (float)x       / (float)gridSize;
		float		x1		= (float)(x + 1) / (float)gridSize;
		float		y0		= (float)y       / (float)gridSize;
		float		y1		= (float)(y + 1) / (float)gridSize;

		float		sx0		= x0 * (float)width;
		float		sx1		= x1 * (float)width;
		float		sy0		= y0 * (float)height;
		float		sy1		= y1 * (float)height;
		float		oosx	= 1.0f / (sx1 - sx0);
		float		oosy	= 1.0f / (sy1 - sy0);

		int			ix0		= deCeilFloatToInt32(sx0 - 0.5f);
		int			ix1		= deCeilFloatToInt32(sx1 - 0.5f);
		int			iy0		= deCeilFloatToInt32(sy0 - 0.5f);
		int			iy1		= deCeilFloatToInt32(sy1 - 0.5f);

		int			v00		= (y * stride) + x;
		int			v01		= (y * stride) + x + 1;
		int			v10		= ((y + 1) * stride) + x;
		int			v11		= ((y + 1) * stride) + x + 1;
		tcu::Vec4	c00		= colors[v00];
		tcu::Vec4	c01		= colors[v01];
		tcu::Vec4	c10		= colors[v10];
		tcu::Vec4	c11		= colors[v11];

		//printf("(%d,%d) -> (%f..%f, %f..%f) (%d..%d, %d..%d)\n", x, y, sx0, sx1, sy0, sy1, ix0, ix1, iy0, iy1);

		for (int iy = iy0; iy < iy1; iy++)
		for (int ix = ix0; ix < ix1; ix++)
		{
			DE_ASSERT(deInBounds32(ix, 0, width));
			DE_ASSERT(deInBounds32(iy, 0, height));

			float				sfx		= (float)ix + 0.5f;
			float				sfy		= (float)iy + 0.5f;
			float				fx1		= deFloatClamp((sfx - sx0) * oosx, 0.0f, 1.0f);
			float				fy1		= deFloatClamp((sfy - sy0) * oosy, 0.0f, 1.0f);

			// Triangle quad interpolation.
			bool				tri		= fx1 + fy1 <= 1.0f;
			float				tx		= tri ? fx1 : (1.0f-fx1);
			float				ty		= tri ? fy1 : (1.0f-fy1);
			const tcu::Vec4&	t0		= tri ? c00 : c11;
			const tcu::Vec4&	t1		= tri ? c01 : c10;
			const tcu::Vec4&	t2		= tri ? c10 : c01;
			tcu::Vec4			color	= t0 + (t1-t0)*tx + (t2-t0)*ty;

			result.setPixel(ix, iy, tcu::RGBA(color));
		}
	}
}

void ShaderRenderCaseInstance::computeFragmentReference (tcu::Surface& result, const QuadGrid& quadGrid)
{
	// Buffer info.
	int					width		= result.getWidth();
	int					height		= result.getHeight();
	bool				hasAlpha	= true;  // \todo [2015-09-07 elecro] add correct alpha check
	ShaderEvalContext	evalCtx		(quadGrid);

	// Render.
	for (int y = 0; y < height; y++)
	for (int x = 0; x < width; x++)
	{
		float sx = ((float)x + 0.5f) / (float)width;
		float sy = ((float)y + 0.5f) / (float)height;

		evalCtx.reset(sx, sy);
		m_evaluator.evaluate(evalCtx);
		// Select either clear color or computed color based on discarded bit.
		tcu::Vec4 color = evalCtx.isDiscarded ? m_clearColor : evalCtx.color;

		if (!hasAlpha)
			color.w() = 1.0f;

		result.setPixel(x, y, tcu::RGBA(color));
	}
}

bool ShaderRenderCaseInstance::compareImages (const tcu::Surface& resImage, const tcu::Surface& refImage, float errorThreshold)
{
	return tcu::fuzzyCompare(m_context.getTestContext().getLog(), "ComparisonResult", "Image comparison result", refImage, resImage, errorThreshold, tcu::COMPARE_LOG_RESULT);
}

} // shaderrendercase
} // vkt
