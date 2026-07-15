#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class RenderTexture
{
public:
	RenderTexture(UINT width, UINT height, RenderSystem* system);
	~RenderTexture();

	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11ShaderResourceView* getShaderResourceView();
	ID3D11DepthStencilView* getDepthStencilView();
	UINT getWidth();
	UINT getHeight();

private:
	ID3D11Texture2D* m_render_target_texture = nullptr;
	ID3D11RenderTargetView* m_rtv = nullptr;
	ID3D11ShaderResourceView* m_srv = nullptr;
	ID3D11Texture2D* m_depth_stencil_buffer = nullptr;
	ID3D11DepthStencilView* m_dsv = nullptr;
	RenderSystem* m_system = nullptr;
	UINT m_width = 0;
	UINT m_height = 0;
};
