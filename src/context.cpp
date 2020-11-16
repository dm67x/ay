#include "context.hpp"
#include <fstream>
#include <streambuf>
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

ShaderProgram* Context::shaderFromFile(const std::string& name, const std::string& vertex, const std::string& fragment) {
    std::ifstream vfile(vertex, std::ifstream::binary);
    if (!vfile.is_open()) {
        spdlog::error("cannot open file: {}", vertex);
        return nullptr;
    }
    std::string vsrc((std::istreambuf_iterator<char>(vfile)), std::istreambuf_iterator<char>());

    std::ifstream ffile(fragment, std::ifstream::binary);
    if (!ffile.is_open()) {
        spdlog::error("cannot open file: {}", fragment);
        return nullptr;
    }
    std::string fsrc((std::istreambuf_iterator<char>(ffile)), std::istreambuf_iterator<char>());
    return shaderFromMemory(name, vsrc, fsrc);
}

Texture2D* Context::texture2DNew(const std::string& name, const std::string& filename) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        spdlog::error("cannot load data from {}", filename);
        return nullptr;
    }

    TextureParameters params;
    params.internalFormat = channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
    params.dataFormat = channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
    params.mipMap = true;
    auto tex = platform->newTexture2D(width, height, data, params);
    stbi_image_free(data);
    textures.insert(std::make_pair(name, tex));
    return tex;
}

Framebuffer* Context::framebufferNew(const std::string& name, const std::array<Texture2D*, 32>& colorAttachments, Renderbuffer* depthStencilAttachment) {
    FramebufferParameters params;
    for (size_t i = 0; i < colorAttachments.size(); i++) {
        params.colorAttachments[i] = colorAttachments[i];
    }
    params.depthStencilAttachment = depthStencilAttachment;
    auto fb = platform->newFramebuffer(params);
    framebuffers.insert(std::make_pair(name, fb));
    return fb;
}