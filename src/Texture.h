#pragma once
namespace PAG
{
	class Texture
	{
		unsigned int id = 0;
		unsigned int texID = 0;
		void initTexture(std::string filename);
		void bindTexture();
	public:
		Texture(std::string filename);
		unsigned int getTexID();
		void activate();
		~Texture();
	};
}
