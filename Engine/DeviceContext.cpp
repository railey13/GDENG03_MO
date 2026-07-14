#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system) : m_device_context(device_context), m_system(system){

}

void DeviceContext::ClearRenderTargetColor(SwapChainPtr swap_chain, f32 red, f32 green, f32 blue, f32 alpha) {
	f32 clear_color[] = {red, green, blue, alpha};
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	m_device_context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, swap_chain->m_dsv);
}

void DeviceContext::setVertexBuffer(VertexBufferPtr vertex_buffer) {
	ui32 stride = vertex_buffer->m_size_vertex;
	ui32 offset = 0;
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);

	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::setIndexBuffer(IndexBufferPtr index_buffer) {
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::setViewportSize(ui32  width, ui32  height) {
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_device_context->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(VertexShaderPtr vertex_shader) {
	m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShaderPtr pixel_shader) {
	m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}

void DeviceContext::setTexutre(VertexShaderPtr vertex_shader, TexturePtr texture) {
	m_device_context->VSSetShaderResources(0, 1, &texture->m_shader_res_view);
}

void DeviceContext::setTexutre(PixelShaderPtr pixel_shader, TexturePtr texture) {
	m_device_context->PSSetShaderResources(0, 1, &texture->m_shader_res_view);
}

void DeviceContext::setConstantBuffer(ConstantBufferPtr constant_buffer) {
	m_device_context->VSSetConstantBuffers(0, 1, &constant_buffer->m_buffer);
	m_device_context->PSSetConstantBuffers(0, 1, &constant_buffer->m_buffer);
}

void DeviceContext::drawTriangleList(ui32  vertex_count, ui32  start_vertex_index) {
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(ui32  index_count, ui32  start_vertex_index, ui32  start_index_location) {
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(ui32  vertex_count, ui32  start_vertex_index){
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

ID3D11DeviceContext* DeviceContext::getContext() {

	return m_device_context;
}

DeviceContext::~DeviceContext() {
	m_device_context->Release();
}
