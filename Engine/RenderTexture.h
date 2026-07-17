#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class RenderTexture {
public:
	RenderTexture(ui32 width, ui32 height, RenderSystem* system);
	~RenderTexture();

	ID3D11RenderTargetView*   getRTV() const;
	ID3D11ShaderResourceView* getSRV() const;
	ID3D11DepthStencilView*   getDSV() const;
	ui32 getWidth()  const;
	ui32 getHeight() const;

private:
	ID3D11Texture2D*           m_color_tex = nullptr;
	ID3D11RenderTargetView*    m_rtv       = nullptr;
	ID3D11ShaderResourceView*  m_srv       = nullptr;
	ID3D11Texture2D*           m_depth_tex = nullptr;
	ID3D11DepthStencilView*    m_dsv       = nullptr;
	RenderSystem*              m_system    = nullptr;
	ui32 m_width  = 0;
	ui32 m_height = 0;

	friend class RenderSystem;
	friend class DeviceContext;
};
