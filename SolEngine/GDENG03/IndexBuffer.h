#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class IndexBuffer
{
public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);
	UINT getSizeIndexList() const;
	~IndexBuffer();

private:
	UINT m_size_list;   // number of indices stored in the buffer

private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
	friend class GraphicsEngine;
	friend class RenderSystem;
};