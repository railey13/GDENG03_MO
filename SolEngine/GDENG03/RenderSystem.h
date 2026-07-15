#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class RenderSystem
{
public:
	RenderSystem();

	bool init();
	bool release();
	~RenderSystem();

public:
	SwapChain* createSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);
	IndexBuffer* createIndexBuffer(void* list_indices, UINT size_list);
	ConstantBuffer* createConstantBuffer(void* buffer, UINT size_buffer);
	VertexShader* createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShader* createPixelShader(const void* shader_byte_code, size_t byte_code_size);
	RenderTexture* createRenderTexture(UINT width, UINT height);

public:
	bool compileVertexShaders(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShaders(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);

	void releaseCompiledShaders();

private:
	DeviceContext* m_imm_device_context;

private:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;

private:
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;

private:
	ID3DBlob* m_blob;    // used by compileVertexShaders
	ID3DBlob* m_psblob;  // used by createShaders for the pixel shader
	// FIX: removed m_vsblob and raw m_vs — vertex shader is now fully
	// managed by the VertexShader wrapper class
	ID3D11PixelShader* m_ps;

private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class ConstantBuffer;
	friend class UIManager;
	friend class RenderTexture;
};