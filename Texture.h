#ifndef TEXTURE_H
#define TEXTURE_H 1

#include <stb/stb_image.h>

#include <glad/glad.h>

#include <string>
#include <memory>

class Texture
{
protected:

    int32_t width;
    int32_t height;
    int32_t num_channels;
    uint8_t *data;
    GLuint handle;

    Texture(const std::string &path);

public:

    ~Texture();

    template<typename... TArgs>
    static std::shared_ptr<Texture> Create(TArgs&... args)
    {
        return std::shared_ptr<Texture>(new Texture(args...));
    }

    void activate();

    void deactivate();

};

#endif /* TEXTURE_H */