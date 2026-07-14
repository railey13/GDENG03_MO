#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class DeviceContext {
public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);

	void ClearRenderTargetColor(SwapChainPtr swap_chain, f32 red, f32 green, f32 blue, f32 alpha);

	void setVertexBuffer(VertexBufferPtr vertex_buffer);
	void setIndexBuffer(IndexBufferPtr index_buffer);
	void setViewportSize(ui32 width, ui32  height);

	void setVertexShader(VertexShaderPtr vertex_shader);
	void setPixelShader(PixelShaderPtr pixel_shader);

	void setTexutre(VertexShaderPtr vertex_shader, TexturePtr texture);
	void setTexutre(PixelShaderPtr pixel_shader, TexturePtr texture);

	void setConstantBuffer(ConstantBufferPtr constant_buffer);
	
	void drawTriangleList(ui32  vertex_count, ui32  start_vertex_index);
	void drawIndexedTriangleList(ui32  index_count, ui32  start_vertex_index, ui32  start_index_location);
	void drawTriangleStrip(ui32  vertex_count, ui32  start_vertex_index);

	ID3D11DeviceContext* getContext();

	~DeviceContext();
private:
	ID3D11DeviceContext* m_device_context;
	RenderSystem* m_system = nullptr;
private:
	friend class ConstantBuffer;

};

