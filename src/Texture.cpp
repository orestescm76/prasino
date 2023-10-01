#include "pch.h"
#include "Texture.h"
#include "Log.h"
#include <lodepng.h>

PAG::Texture::Texture(std::string filename, TextureType type): textureType(type)
{
	initTexture(filename);
	if (textureType == TextureType::NORMAL_MAP)
		texID = 1;
}

PAG::Texture::Texture(const Texture& orig): id(orig.id), texID(orig.texID), textureType(orig.textureType)
{
}

void PAG::Texture::setTexID(unsigned id)
{
	texID = id;
}

unsigned int PAG::Texture::getTexID()
{
	return texID;
}

unsigned int PAG::Texture::getOpenGLID()
{
	return id;
}

void PAG::Texture::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void PAG::Texture::activate()
{
	glActiveTexture(GL_TEXTURE0 + texID);
	bindTexture();
}

PAG::TextureType PAG::Texture::type()
{
	return textureType;
}

void PAG::Texture::initTexture(std::string filename)
{
	std::vector<unsigned char> image;
	unsigned int h, w;
	unsigned int error = lodepng::decode(image, w, h, filename);
	Log::getInstance()->printMessage(msgType::INFO, "Loading "+filename);
	if (error)
	{
		Log::getInstance()->printMessage(msgType::LOG_ERROR, "Cannot load " + filename);
		Log::getInstance()->printMessage(msgType::INFO, lodepng_error_text(error));
		throw std::runtime_error("PAG::Texture::initTexture(std::string filename)");
	}
	//loaded image!
	//its reversed
	
	unsigned char* imgPrt = &image[0];
	int compColor = 4;
	int incW = w * compColor;
	unsigned char* top = nullptr;
	unsigned char* bot = nullptr;
	unsigned char temp = 0;
	for (int i = 0; i < h/2; i++)
	{
		top = imgPrt + i * incW;
		bot = imgPrt + (h - i - 1) * incW;
		for (int j = 0; j < incW; j++)
		{
			temp = *top;
			*top = *bot;
			*bot = temp;
			++top;
			++bot;
		}
	}
	//We caN create the texture
	glGenTextures(1, &id);
	bindTexture();
	//OpenGL params
	//minification.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//wrap s
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//wrap t
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// send to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image.data());
	glGenerateTextureMipmap(id);
}


PAG::Texture::~Texture()
{
	glDeleteTextures(1, &id);
}
