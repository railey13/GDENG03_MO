#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexBuffer
{
public:
	VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader, RenderSystem* system);
	VertexBuffer(RenderSystem* system = nullptr);
	// Dynamic buffer (CPU-writable each frame) — used by particle system
	bool loadDynamic(UINT size_vertex, UINT max_vertices, void* shader_byte_code, UINT size_byte_shader);
	// Upload new data into a dynamic buffer each frame
	bool updateDynamic(void* data, UINT vertex_count);
	UINT getSizeVertexList() const;
	~VertexBuffer();

private:
	UINT m_size_vertex;
	UINT m_size_list;       // current live vertex count (updated each frame)
	UINT m_max_vertices;    // GPU buffer capacity (set once in loadDynamic, never shrinks)

private:
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;
	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext; // FIX: was 'deviceContext' — friendship never applied,
	// causing all private member access errors in DeviceContext.cpp
	friend class RenderSystem;
	friend class GraphicsEngine;
};