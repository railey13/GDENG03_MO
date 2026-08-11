#include "TextureComponent.h"
#include "GraphicsEngine.h"
#include "Debug.h"

TextureComponent::TextureComponent(){
	m_texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/white.png");
}

TextureComponent::~TextureComponent() {

}

void TextureComponent::setTexture(const TexturePtr tex)  {
	m_texture = tex;
}

void TextureComponent::setTexturePath(std::string path) {
	std::wstring wpath(path.begin(), path.end());
	auto temp = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(wpath.c_str());
	if (!temp) {
		Debug::Error(path + " not found");
		return;
	}

	m_texture = temp;
	texPath = path;
}

std::string TextureComponent::getTexturePath()
{
	return texPath;
}
