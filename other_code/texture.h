#pragma once

#include <gl/glew.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& fileName);
	~Texture();

	void bind();
	void unbind();

	inline GLuint getId() const { return id; }
private:
	GLuint id;
	unsigned char* buffer;
	int width, height, BPP;

};