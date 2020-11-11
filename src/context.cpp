#include "context.hpp"
#include "math.hpp"
#include <iostream>
#include <fstream>
#include <stb_image.h>

Context::~Context() {
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        delete it->second;
    }

    for (auto it = textures.begin(); it != textures.end(); it++) {
        delete it->second;
    }

    for (auto it = renderbuffers.begin(); it != renderbuffers.end(); it++) {
        delete it->second;
    }

    for (auto it = framebuffers.begin(); it != framebuffers.end(); it++) {
        delete it->second;
    }
}

void Context::clear(float r, float g, float b, float a) const {
    platform->clear(r, g, b, a);
}

void Context::viewport(int x, int y, int w, int h) const {
    platform->viewport(x, y, w, h);
}

void Context::shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment) {
    auto program = platform->newProgram(vertex, fragment);
    shaders.insert(std::make_pair(name, program));
}

void Context::shaderFromFile(const std::string& name, const std::string& vertex, const std::string& fragment) {
    std::string vsrc, fsrc;
    
    std::ifstream file(vertex, std::ifstream::binary);
    if (!file.is_open()) {
        std::cerr << "cannot open file: " << vertex << std::endl;
        return;
    }

    file.seekg(0, file.end);
    auto length = file.tellg();
    file.seekg(0, file.beg);
    file.read(&vsrc[0], length);
    file.close();

    file.open(fragment, std::ifstream::binary);
    if (!file.is_open()) {
        std::cerr << "cannot open file: " << fragment << std::endl;
        return;
    }

    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);
    file.read(&fsrc[0], length);
    file.close();

    shaderFromMemory(name, vsrc, fsrc);
}

void Context::shaderDestroy(const std::string& name) {
    auto it = shaders.find(name);
    if (it != shaders.end()) {
        delete it->second;
        shaders.erase(it);
    }
}

void Context::shaderUse(const std::string& name) {
    auto shader = shaders.find(name);
    if (shader != shaders.end()) {
        currentShader = shader->second;
    }
    currentShader->use();
}

void Context::shaderUniform(const std::string& name, float value) const {
    currentShader->uniform1f(name, value);
}

void Context::shaderUniform(const std::string& name, int value) const {
    currentShader->uniform1i(name, value);
}

void Context::shaderUniform(const std::string& name, const Vec3& value) const {
    currentShader->uniform3f(name, new float[3] { value.x, value.y, value.z });
}

VertexArrayObject* Context::vertexArrayObjectNew() const {
    return platform->newVertexArrayObject();
}

Buffer* Context::bufferNew() const {
    return platform->newBuffer();
}

void Context::texture2DNew(const std::string& name, int width, int height) {
    auto tex = platform->newTexture2D(width, height, nullptr, TextureParameters());
    textures.insert(std::make_pair(name, tex));
}

void Context::texture2DNew(const std::string& name, const std::string& filename) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "cannot load data from " << filename << std::endl;
        return;
    }

    TextureParameters params;
    params.dataFormat = channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
    params.mipMap = true;
    auto tex = platform->newTexture2D(width, height, data, params);
    stbi_image_free(data);
    textures.insert(std::make_pair(name, tex));
}

void Context::texture2DDestroy(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        delete it->second;
        textures.erase(it);
    }
}

Texture2D* Context::texture2DGet(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

void Context::renderbufferNew(const std::string& name, int width, int height) {
    auto rb = platform->newRenderbuffer(width, height);
    renderbuffers.insert(std::make_pair(name, rb));
}

void Context::renderbufferDestroy(const std::string& name) {
    auto it = renderbuffers.find(name);
    if (it != renderbuffers.end()) {
        delete it->second;
        renderbuffers.erase(it);
    }
}

Renderbuffer* Context::renderbufferGet(const std::string& name) const {
    auto it = renderbuffers.find(name);
    if (it != renderbuffers.end()) {
        return it->second;
    }
    return nullptr;
}

void Context::framebufferNew(const std::string& name, const std::array<Texture2D*, 32>& colorAttachments, Renderbuffer* depthStencilAttachment) {
    FramebufferParameters params;
    for (size_t i = 0; i < colorAttachments.size(); i++) {
        params.colorAttachments[i] = colorAttachments[i];
    }
    params.depthStencilAttachment = depthStencilAttachment;
    auto fb = platform->newFramebuffer(params);
    framebuffers.insert(std::make_pair(name, fb));
}

void Context::framebufferDestroy(const std::string& name) {
    auto it = framebuffers.find(name);
    if (it != framebuffers.end()) {
        delete it->second;
        framebuffers.erase(it);
    }
}

Framebuffer* Context::framebufferGet(const std::string& name) const {
    auto it = framebuffers.find(name);
    if (it != framebuffers.end()) {
        return it->second;
    }
    return nullptr;
}