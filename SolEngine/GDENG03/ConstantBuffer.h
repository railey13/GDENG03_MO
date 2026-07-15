#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system);
	void update(DeviceContext* context, void* buffer);
	~ConstantBuffer();

private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext; // FIX: was 'deviceContext' — friendship never applied,
	// causing all private member access errors in DeviceContext.cpp
	friend class RenderSystem;
	friend class GraphicsEngine;
};