#pragma once
#include "Resource.h"
#include <d3d11.h>

class Texture : public Resource{
public:
	Texture(const wchar_t* full_path);
	~Texture();
	ID3D11ShaderResourceView* getSRV() { return m_shader_res_view; }
private:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_res_view = nullptr;
private:
	friend class DeviceContext;
};

