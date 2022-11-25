#pragma once
// Managed OpenGL types

#include <glad/glad.h>
#include <memory>

struct VertexArrayResource
{
    GLuint ID;

    VertexArrayResource(const VertexArrayResource&) = delete;
    VertexArrayResource() { glGenVertexArrays(1, &ID); };
    ~VertexArrayResource() { glDeleteVertexArrays(1, &ID);  };

    void bind() const { glBindVertexArray(ID); };
};

template <unsigned int N = 1>
struct BufferResource
{
    GLuint ID[N];
    BufferResource(const BufferResource&) = delete;
    BufferResource() { glGenBuffers(N, ID); };
    ~BufferResource() { glDeleteBuffers(N, ID);  };
};

struct TextureResource
{
    GLuint ID;
    TextureResource(const TextureResource&) = delete;
    TextureResource() { glGenTextures(1, &ID); }
    ~TextureResource() { glDeleteTextures(1, &ID); }
    void bind(const GLenum& target = GL_TEXTURE_2D) const { glBindTexture(target, ID); };
};

class VertexArray : public std::shared_ptr<VertexArrayResource>
{
public:
    VertexArray() : std::shared_ptr<VertexArrayResource>(std::make_shared<VertexArrayResource>()) {}
};

template <unsigned int N = 1>
class Buffer : public std::shared_ptr<BufferResource<N>>
{
public:
    Buffer() : std::shared_ptr<BufferResource<N>>(std::make_shared<BufferResource<N>>()) {}
};

class Texture : public std::shared_ptr<TextureResource>
{
public:
    Texture() : std::shared_ptr<TextureResource>(std::make_shared<TextureResource>()) {}
};
