#include "RenderTexture.h"
#include "RenderSystem.h"
#include <exception>

RenderTexture::RenderTexture(ui32 width, ui32 height, RenderSystem* system)
	: m_width(width), m_height(height), m_system(system)
{
	ID3D11Device* device = m_system->m_d3d_device;

	// --- Color render target ---
	D3D11_TEXTURE2D_DESC color_desc = {};
	color_desc.Width            = width;
	color_desc.Height           = height;
	color_desc.MipLevels        = 1;
	color_desc.ArraySize        = 1;
	color_desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	color_desc.SampleDesc.Count = 1;
	color_desc.Usage            = D3D11_USAGE_DEFAULT;
	color_desc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	if (FAILED(device->CreateTexture2D(&color_desc, nullptr, &m_color_tex)))
		throw std::exception("RenderTexture: failed to create color texture");

	if (FAILED(device->CreateRenderTargetView(m_color_tex, nullptr, &m_rtv)))
		throw std::exception("RenderTexture: failed to create RTV");

	if (FAILED(device->CreateShaderResourceView(m_color_tex, nullptr, &m_srv)))
		throw std::exception("RenderTexture: failed to create SRV");

	// --- Depth buffer ---
	D3D11_TEXTURE2D_DESC depth_desc = {};
	depth_desc.Width            = width;
	depth_desc.Height           = height;
	depth_desc.MipLevels        = 1;
	depth_desc.ArraySize        = 1;
	depth_desc.Format           = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_desc.SampleDesc.Count = 1;
	depth_desc.Usage            = D3D11_USAGE_DEFAULT;
	depth_desc.BindFlags        = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(device->CreateTexture2D(&depth_desc, nullptr, &m_depth_tex)))
		throw std::exception("RenderTexture: failed to create depth texture");

	if (FAILED(device->CreateDepthStencilView(m_depth_tex, nullptr, &m_dsv)))
		throw std::exception("RenderTexture: failed to create DSV");
}

RenderTexture::~RenderTexture()
{
	if (m_dsv)       m_dsv->Release();
	if (m_depth_tex) m_depth_tex->Release();
	if (m_srv)       m_srv->Release();
	if (m_rtv)       m_rtv->Release();
	if (m_color_tex) m_color_tex->Release();
}

ID3D11RenderTargetView*   RenderTexture::getRTV()    const { return m_rtv; }
ID3D11ShaderResourceView* RenderTexture::getSRV()    const { return m_srv; }
ID3D11DepthStencilView*   RenderTexture::getDSV()    const { return m_dsv; }
ui32                      RenderTexture::getWidth()  const { return m_width; }
ui32                      RenderTexture::getHeight() const { return m_height; }
