#include "TextureComponent.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image.h"
#include "glad/gl.h"

void TextureComponent::loadTexture(const char* texturePath)
{
    unsigned char* imageData;
    int texWidth, texHeight, bpp;
    imageData = stbi_load(texturePath, &texWidth, &texHeight, &bpp, 4);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        texWidth,
        texHeight,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        imageData);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(imageData);
}

void TextureComponent::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, this->textureID);
}
