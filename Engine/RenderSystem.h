#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class RenderSystem {
private:
	RenderSystem();

	~RenderSystem();
public:
	SwapChainPtr createSwapChain(HWND hwnd, ui32  width, ui32  height);
	DeviceContextPtr getImmediateDeviceContext();
	VertexBufferPtr createVertexBuffer(void* list_vertices, ui32  size_vertex, ui32  size_list, void* shader_byte_code, ui32  size_byte_shader);
	IndexBufferPtr createIndexBuffer(void* list_indices, ui32  size_list);
	ConstantBufferPtr createConstantBuffer(void* buffer, ui32  size_buffer);
	VertexShaderPtr createVertexShader(void* shader_byte_code, size_t byte_code_size);
	PixelShaderPtr createPixelShader(void* shader_byte_code, size_t byte_code_size);
	RenderTexturePtr createRenderTexture(ui32 width, ui32 height);

	ID3D11Device* getD11Device();
public:
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);

	void releaseCompiledShader();
public:
	ID3D11BlendState* createAlphaBlendState();
public:
	ID3D11BlendState* m_alpha_blend_state = nullptr;
private:
	DeviceContextPtr m_imm_device_context; 
private:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	ID3D11DeviceContext* m_imm_context;
private:
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
private:
	ID3DBlob* m_blob = nullptr;

	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class GraphicsEngine;
	friend class Texture;
	friend class RenderTexture;
};

