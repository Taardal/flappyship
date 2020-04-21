#include "system/Log.h"
#include "Texture.h"
#include <glad/glad.h>

namespace storytime
{
    Texture::Texture(const Image& image)
            : width(image.Width), height(image.Height)
    {
        if (image.Channels == 3)
        {
            format = GL_RGB;
            internalFormat = GL_RGB8;
        }
        else
        {
            format = GL_RGBA;
            internalFormat = GL_RGBA8;
        }
        CreateTexture();
        SetPixels(image.Pixels);
    }

    Texture::Texture(uint32_t width, uint32_t height)
            : width(width), height(height)
    {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
        CreateTexture();
    }

    Texture::~Texture()
    {
        ST_GL_CALL(ST_TAG, glDeleteTextures(1, &id));
    }

    void Texture::SetPixels(const void* pixels) const
    {
        ST_GL_CALL(ST_TAG, glTexImage2D(
                TARGET,
                LEVEL_OF_DETAIL,
                internalFormat,
                width,
                height,
                BORDER,
                format,
                GL_UNSIGNED_BYTE,
                pixels
        ));
    }

    void Texture::Bind(uint32_t textureSlot) const
    {
        ST_GL_CALL(ST_TAG, glActiveTexture(GL_TEXTURE0 + textureSlot));
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, id));
    }

    void Texture::Unbind() const
    {
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, 0));
    }

    void Texture::CreateTexture()
    {
        ST_GL_CALL(ST_TAG, glGenTextures(1, &id));
        ST_GL_CALL(ST_TAG, glBindTexture(TARGET, id));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_REPEAT));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_REPEAT));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        ST_GL_CALL(ST_TAG, glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }

}

