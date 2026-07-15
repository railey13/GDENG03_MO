#include "RenderTexture.h"
#include "RenderSystem.h"
#include <exception>

RenderTexture::RenderTexture(UINT width, UINT height, RenderSystem* system)
	: m_width(width)
	, m_height(height)
	, m_system(system)
{
	ID3D11Device* device = m_system->m_d3d_device;

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = width;
	texture_desc.Height = height;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	if (FAILED(device->CreateTexture2D(&texture_desc, nullptr, &m_render_target_texture)))
		throw std::exception("RenderTexture: failed to create target texture");

	if (FAILED(device->CreateRenderTargetView(m_render_target_texture, nullptr, &m_rtv)))
		throw std::exception("RenderTexture: failed to create RTV");

	if (FAILED(device->CreateShaderResourceView(m_render_target_texture, nullptr, &m_srv)))
		throw std::exception("RenderTexture: failed to create SRV");

	D3D11_TEXTURE2D_DESC depth_desc = {};
	depth_desc.Width = width;
	depth_desc.Height = height;
	depth_desc.MipLevels = 1;
	depth_desc.ArraySize = 1;
	depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_desc.SampleDesc.Count = 1;
	depth_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(device->CreateTexture2D(&depth_desc, nullptr, &m_depth_stencil_buffer)))
		throw std::exception("RenderTexture: failed to create depth buffer");

	if (FAILED(device->CreateDepthStencilView(m_depth_stencil_buffer, nullptr, &m_dsv)))
		throw std::exception("RenderTexture: failed to create DSV");
}

RenderTexture::~RenderTexture()
{
	if (m_dsv) m_dsv->Release();
	if (m_depth_stencil_buffer) m_depth_stencil_buffer->Release();
	if (m_srv) m_srv->Release();
	if (m_rtv) m_rtv->Release();
	if (m_render_target_texture) m_render_target_texture->Release();
}

ID3D11RenderTargetView* RenderTexture::getRenderTargetView() { return m_rtv; }
ID3D11ShaderResourceView* RenderTexture::getShaderResourceView() { return m_srv; }
ID3D11DepthStencilView* RenderTexture::getDepthStencilView() { return m_dsv; }
UINT RenderTexture::getWidth() { return m_width; }
UINT RenderTexture::getHeight() { return m_height; }
