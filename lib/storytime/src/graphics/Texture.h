#pragma once

#include <string>

namespace storytime
{
    struct Image
    {
        unsigned char* Pixels;
        int32_t Width;
        int32_t Height;
        int32_t Channels;
    };

    class Texture
    {
    private:
        static const int32_t TARGET = GL_TEXTURE_2D;
        static const int32_t LEVEL_OF_DETAIL = 0;
        static const int32_t BORDER = 0;

        uint32_t id = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        int32_t format = 0;
        int32_t internalFormat = 0;

    public:
        explicit Texture(const Image& image);

        Texture(uint32_t width, uint32_t height);

        ~Texture();

        void SetPixels(const void* data) const;

        void Bind(uint32_t textureUnit = 0) const;

        void Unbind() const;

    private:
        void CreateTexture();
    };
}


