#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::string &path)
{
    data = stbi_load(path.c_str(), &width, &height, &num_channels, 0);

    if (data)
    {
        glGenTextures(1, &handle);
    
        activate();
    
        GLint format;
        if (num_channels == 1)
        {
            format = GL_RED;
        }
        else if (num_channels == 2)
        {
            format = GL_RG;
        }
        else if (num_channels == 3)
        {
            format = GL_RGB;
        }
        else if (num_channels == 4)
        {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        //stbi_image_free(data);
        //data = nullptr;

        deactivate();

    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle);
    handle = 0;

    if (data)
    {
        stbi_image_free(data);
        data = nullptr;
    }
}

void Texture::activate()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::deactivate()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}