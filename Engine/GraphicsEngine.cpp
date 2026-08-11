#include "GraphicsEngine.h"
#include <exception>

GraphicsEngine* GraphicsEngine::sharedInstance = NULL;

GraphicsEngine* GraphicsEngine::get() {
    return sharedInstance;
}

void GraphicsEngine::initialize() {
    if(sharedInstance != NULL) throw std::exception("Graphcis Engine already exists");

    sharedInstance = new GraphicsEngine();
}

void GraphicsEngine::destroy() {
    if (sharedInstance == NULL) return;

    delete sharedInstance;
}

RenderSystem* GraphicsEngine::getRenderSystem() {
    return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager() {
    return m_tex_manager;
}

MeshManager* GraphicsEngine::getMeshManager() {
    return m_mesh_manager;
}

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size) {
    *byte_code = m_mesh_layout_byte_code;
    *size = m_mesh_layout_size;
}

GraphicsEngine::GraphicsEngine() {
    try {
        m_render_system = new RenderSystem();
    }
    catch (...) {
        throw std::exception("Render System did not initiate successfully");
    }

    try {
        m_tex_manager = new TextureManager();
    }
    catch (...) {
        throw std::exception("Texture manager did not initiate successfully");
    }

    try {
        m_mesh_manager = new MeshManager();
    }
    catch (...) {
        throw std::exception("Mesh manager did not initiate successfully");
    }

    void* shader_byte_code = nullptr;
    size_t size_shader= 0;

    m_render_system->compileVertexShader(L"Engine/VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
    ::memcpy(m_mesh_layout_byte_code, shader_byte_code, size_shader);
    m_mesh_layout_size = size_shader;

    m_render_system->releaseCompiledShader();
}

GraphicsEngine::~GraphicsEngine() {
    sharedInstance = nullptr;
    delete m_mesh_manager;
    delete m_tex_manager;
    delete m_render_system;
}
