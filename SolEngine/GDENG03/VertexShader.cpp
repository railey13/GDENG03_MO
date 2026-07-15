#include "VertexShader.h"
#include "Prerequisites.h"
#include "RenderSystem.h"
#include <exception>

VertexShader::VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system)
	: m_system(system)
	, m_vs(nullptr)
{
	if (FAILED(m_system->m_d3d_device->CreateVertexShader(
		shader_byte_code, byte_code_size, nullptr, &m_vs)))
	{
		throw std::exception("VertexShader not created successfully");
	}
}

VertexShader::~VertexShader()
{
	if (m_vs) { m_vs->Release(); m_vs = nullptr; }
}