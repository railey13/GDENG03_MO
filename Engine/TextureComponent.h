#pragma once
#include "Component.h"
#include "Prerequisites.h"

class TextureComponent : public Component {
public:
	TextureComponent();
	~TextureComponent();
public:
	void setTexture(TexturePtr tex);
	void setTexturePath(std::string path);
	TexturePtr getTexture() const { return m_texture; }
private:
	TexturePtr m_texture;
};

