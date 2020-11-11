#include "context.hpp"
#include "math.hpp"
#include <iostream>
#include <fstream>
#include <stb_image.h>

Context::~Context() {
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        Platform::shaderDestroy(it->second);
    }

    for (auto it = textures.begin(); it != textures.end(); it++) {
        Platform::textureDestroy(it->second);
    }

    for (auto it = renderbuffers.begin(); it != renderbuffers.end(); it++) {
        Platform::renderbufferDestroy(it->second);
    }

    for (auto it = framebuffers.begin(); it != framebuffers.end(); it++) {
        Platform::framebufferDestroy(it->second);
    }
}

void Context::clear(float r, float g, float b, float a) const {
    Platform::clear(r, g, b, a);
}

void Context::viewport(int x, int y, int w, int h) const {
    Platform::viewport(x, y, w, h);
}

PlatformId Context::shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment) {
    PlatformId id = Platform::shaderNew(vertex, fragment);
    if (id != 0) {
        shaders.insert(std::make_pair(name, id));
    }
    return id;
}

PlatformId Context::shaderFromFile(const std::string& name, const std::string& vertex, const std::string& fragment) {
    std::string vsrc, fsrc;
    
    std::ifstream file(vertex, std::ifstream::binary);
    if (!file.is_open()) {
        std::cerr << "cannot open file: " << vertex << std::endl;
        return 0;
    }

    file.seekg(0, file.end);
    auto length = file.tellg();
    file.seekg(0, file.beg);
    file.read(&vsrc[0], length);
    file.close();

    file.open(fragment, std::ifstream::binary);
    if (!file.is_open()) {
        std::cerr << "cannot open file: " << fragment << std::endl;
        return 0;
    }

    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);
    file.read(&fsrc[0], length);
    file.close();

    return shaderFromMemory(name, vsrc, fsrc);
}

void Context::shaderDestroy(const std::string& name) {
    auto it = shaders.find(name);
    if (it != shaders.end()) {
        Platform::shaderDestroy(it->second);
        shaders.erase(it);
    }
}

void Context::shaderUse(const std::string& name) {
    auto shader = shaders.find(name);
    if (shader != shaders.end()) {
        currentShader = shader->second;
    }
    Platform::shaderUse(currentShader);
}

void Context::shaderUniform(const std::string& name, float value) const {
    Platform::shaderUniform1f(currentShader, name, value);
}

void Context::shaderUniform(const std::string& name, int value) const {
    Platform::shaderUniform1i(currentShader, name, value);
}

void Context::shaderUniform(const std::string& name, const Vec3& value) const {
    Platform::shaderUniform3fv(currentShader, name, new float[3] { value.x, value.y, value.z });
}

PlatformId Context::vaoNew() const {
    return Platform::vertexArrayNew();
}

void Context::vaoDestroy(PlatformId id) const {
    Platform::vertexArrayDestroy(id);
}

void Context::vaoUse(PlatformId id) const {
    Platform::vertexArrayUse(id);
}

PlatformId Context::bufferNew() const {
    return Platform::bufferNew();
}

void Context::bufferDestroy(PlatformId id) const {
    Platform::bufferDestroy(id);
}

void Context::bufferUse(PlatformId id, Platform::BufferMode mode) const {
    Platform::bufferUse(id, mode);
}

void Context::bufferData(Platform::BufferMode mode, size_t size, const void* data, Platform::BufferTarget target) const {
    Platform::bufferData(mode, size, data, target);
}

void Context::bufferAttribArray(PlatformId id, int size, Platform::AttribType type, size_t stride, const void* ptr) {
    Platform::vertexAttribArray(id, size, type, stride, ptr);
}

void Context::drawArrays(Platform::DrawMode mode, int first, size_t count) const {
    Platform::drawArrays(mode, first, count);
}

void Context::drawElements(Platform::DrawMode mode, size_t count, Platform::AttribType type, const void* indices) {
    Platform::drawElements(mode, count, type, indices);
}

void Context::textureNew(const std::string& name, int width, int height) {
    PlatformId id = Platform::textureNew(width, height, nullptr, Platform::TextureParameters());
    textures.insert(std::make_pair(name, id));
}

void Context::textureNew(const std::string& name, const std::string& filename) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "cannot load data from " << filename << std::endl;
        return;
    }

    Platform::TextureParameters params;
    params.dataFormat = channels == 4 ? Platform::TextureFormat::RGBA : Platform::TextureFormat::RGB;
    params.mipMap = true;
    PlatformId id = Platform::textureNew(width, height, data, params);
    stbi_image_free(data);
    textures.insert(std::make_pair(name, id));
}

void Context::textureDestroy(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        Platform::textureDestroy(it->second);
        textures.erase(it);
    }
}

void Context::textureUse(const std::string& name, unsigned char slot) const {
    auto it = textures.find(name);
    if (it != textures.end()) {
        Platform::textureUse(it->second, slot);
    }
}

PlatformId Context::textureGet(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    return 0;
}

void Context::renderbufferNew(const std::string& name, int width, int height) {
    PlatformId id = Platform::renderbufferNew(width, height);
    renderbuffers.insert(std::make_pair(name, id));
}

void Context::renderbufferDestroy(const std::string& name) {
    auto it = renderbuffers.find(name);
    if (it != renderbuffers.end()) {
        Platform::renderbufferDestroy(it->second);
        renderbuffers.erase(it);
    }
}

PlatformId Context::renderbufferGet(const std::string& name) const {
    auto it = renderbuffers.find(name);
    if (it != renderbuffers.end()) {
        return it->second;
    }
    return 0;
}

void Context::framebufferNew(const std::string& name, const std::vector<PlatformId>& colorAttachments, PlatformId depthStencilAttachment) {
    Platform::FramebufferParameters params;
    if (colorAttachments.size() > 32) {
        std::cerr << "framebuffer cannot have more than 32 color attachments" << std::endl;
        return;
    }

    for (size_t i = 0; i < colorAttachments.size(); i++) {
        params.colorAttachments[i] = colorAttachments[i];
    }

    params.depthStencilAttachment = depthStencilAttachment;

    PlatformId id = Platform::framebufferNew(params);
    if (id != 0) {
        framebuffers.insert(std::make_pair(name, id));
    }
}

void Context::framebufferDestroy(const std::string& name) {
    auto it = framebuffers.find(name);
    if (it != framebuffers.end()) {
        Platform::framebufferDestroy(it->second);
        framebuffers.erase(it);
    }
}

void Context::framebufferUse(const std::string& name) const {
    if (name.size() == 0) {
        Platform::framebufferUse(0);
        return;
    }

    auto it = framebuffers.find(name);
    if (it != framebuffers.end()) {
        Platform::framebufferUse(it->second);
    }
}