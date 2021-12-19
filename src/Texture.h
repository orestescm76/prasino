#pragma once
namespace PAG
{
	enum class TextureType
	{
		IMAGE,
		NORMAL_MAP
	};
	class Texture
	{
		unsigned int id = 0;
		unsigned int texID = 0;
		void initTexture(std::string filename);
		void bindTexture();
		TextureType textureType;
	public:
		Texture(std::string filename, TextureType type);
		void setTexID(unsigned id);
		unsigned int getTexID();
		unsigned int getOpenGLID();
		void activate();
		TextureType type();
		~Texture();
	};
}
