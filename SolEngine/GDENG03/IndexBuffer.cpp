#include "IndexBuffer.h"
#include "Prerequisites.h"
#include "RenderSystem.h"
#include <exception>


IndexBuffer::IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system)
	: m_system(system)
	, m_buffer(nullptr)
	, m_size_list(size_list)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage          = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth      = sizeof(UINT) * size_list; // each index is a 32-bit UINT
	buff_desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;  // index buffer, not vertex buffer
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags      = 0;
	buff_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem       = list_indices;
	init_data.SysMemPitch   = 0;
	init_data.SysMemSlicePitch = 0;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("IndexBuffer not created successfully");
	}
}

UINT IndexBuffer::getSizeIndexList() const
{
	return m_size_list;
}

IndexBuffer::~IndexBuffer()
{
	if (m_buffer) { m_buffer->Release(); m_buffer = nullptr; }
}