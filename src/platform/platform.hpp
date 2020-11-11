#pragma once

#include <string>

class ShaderProgram;
class Texture2D;
struct TextureParameters;
class Renderbuffer;
class Framebuffer;
struct FramebufferParameters;
class VertexArrayObject;
class Buffer;

struct Platform {
    virtual const std::string getVendor() = 0;
    virtual const std::string getVersion() = 0;
    virtual void clear(float r, float g, float b, float alpha) = 0;
    virtual void viewport(int x, int y, int w, int h) = 0;
    virtual ShaderProgram* newProgram(const std::string& vertex, const std::string& fragment) = 0;
    virtual Texture2D* newTexture2D(int width, int height, const void* data, const TextureParameters& params) = 0;
    virtual Renderbuffer* newRenderbuffer(int width, int height) = 0;
    virtual Framebuffer* newFramebuffer(const FramebufferParameters& params) = 0;
    virtual VertexArrayObject* newVertexArrayObject() = 0;
    virtual Buffer* newBuffer() = 0;
};