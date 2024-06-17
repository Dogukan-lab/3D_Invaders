#include "TextureComponent.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image.h"
#include "glad/gl.h"

void TextureComponent::loadTexture(const std::string& texturePath)
{
    texPath = texturePath;
    stbi_set_flip_vertically_on_load(1);
    int width, height, BPP;
    unsigned char* buffer = stbi_load(texturePath.c_str(), &width, &height, &BPP, 4);
    if (buffer == nullptr)
    {
        std::cout << stbi_failure_reason() << std::endl;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // S and T are like X and Y for textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (buffer) {
        stbi_image_free(buffer);
    }
}

void TextureComponent::bindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, this->textureID);
}
void TextureComponent::unbindTexture() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
