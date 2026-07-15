#include "PixelShader.h"
#include "Prerequisites.h"
#include "RenderSystem.h"
#include <exception>

PixelShader::PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system)
	: m_system(system)
	, m_ps(nullptr)
{
	if (FAILED(m_system->m_d3d_device->CreatePixelShader(
		shader_byte_code, byte_code_size, nullptr, &m_ps)))
	{
		throw std::exception("PixelShader not created successfully");
	}
}

PixelShader::~PixelShader()
{
	if (m_ps) { m_ps->Release(); m_ps = nullptr; }
}
