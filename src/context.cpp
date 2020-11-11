#include "context.hpp"
#include "math.hpp"
#include <iostream>
#include <fstream>
#include <stb_image.h>

Context::~Context() {
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        Platform::destroyProgram(it->second);
    }

    for (auto it = textures.begin(); it != textures.end(); it++) {
        Platform::destroyTexture(it->second);
    }
}

void Context::clear(float r, float g, float b, float a) const {
    Platform::clear(r, g, b, a);
}

PlatformId Context::shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment) {
    PlatformId id = Platform::createProgram();
    PlatformId vid = Platform::createVertexShader();
    PlatformId fid = Platform::createFragmentShader();
    std::string log = "";
    
    Platform::shaderSource(vid, vertex);
    Platform::compileShader(vid);
    log = Platform::getShaderLog(vid);
    if (log.size() > 0) {
        std::cerr << "Vertex: " << log << std::endl;
        return 0;
    }

    Platform::shaderSource(fid, fragment);
    Platform::compileShader(fid);
    log = Platform::getShaderLog(fid);
    if (log.size() > 0) {
        std::cerr << "Fragment: " << log << std::endl;
        return 0;
    }

    Platform::attachShader(id, vid);
    Platform::attachShader(id, fid);
    Platform::linkProgram(id);
    log = Platform::getProgramLog(id);
    if (log.size() > 0) {
        std::cerr << "Program: " << log << std::endl;
        return 0;
    }
    Platform::detachShader(id, vid);
    Platform::detachShader(id, fid);
    Platform::destroyShader(vid);
    Platform::destroyShader(fid);
    shaders.insert(std::make_pair(name, id));
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
        Platform::destroyShader(it->second);
        shaders.erase(it);
    }
}

void Context::shaderUse(const std::string& name) {
    auto shader = shaders.find(name);
    if (shader != shaders.end()) {
        currentShader = shader->second;
    }
    Platform::useProgram(currentShader);
}

void Context::shaderUniform(const std::string& name, float value) const {
    Platform::uniform1f(currentShader, name, value);
}

void Context::shaderUniform(const std::string& name, int value) const {
    Platform::uniform1i(currentShader, name, value);
}

void Context::shaderUniform(const std::string& name, const Vec3& value) const {
    Platform::uniform3fv(currentShader, name, new float[3] { value.x, value.y, value.z });
}

PlatformId Context::vaoNew() const {
    return Platform::createVertexArray();
}

void Context::vaoDestroy(PlatformId id) const {
    Platform::destroyVertexArray(id);
}

void Context::vaoBind(PlatformId id) const {
    Platform::bindVertexArray(id);
}

PlatformId Context::bufferNew() const {
    return Platform::createBuffer();
}

void Context::bufferDestroy(PlatformId id) const {
    Platform::destroyBuffer(id);
}

void Context::bufferBind(PlatformId id, Platform::BufferMode mode) const {
    Platform::bindBuffer(id, mode);
}

void Context::bufferData(Platform::BufferMode mode, size_t size, const void* data, Platform::BufferTarget target) const {
    OpenGL::bufferData(mode, size, data, target);
}

void Context::bufferAttribArray(PlatformId id, int size, Platform::AttribType type, size_t stride, const void* ptr) {
    OpenGL::vertexAttribArray(id, size, type, stride, ptr);
}

void Context::drawArrays(Platform::DrawMode mode, int first, size_t count) const {
    OpenGL::drawArrays(mode, first, count);
}

void Context::drawElements(Platform::DrawMode mode, size_t count, Platform::AttribType type, const void* indices) {
    OpenGL::drawElements(mode, count, type, indices);
}

PlatformId Context::textureNew(const std::string& name, int width, int height) {
    PlatformId id = Platform::createTexture();
    Platform::textureBind(id);
    Platform::textureMinParameter(id, Platform::TextureFiltering::LINEAR);
    Platform::textureMagParameter(id, Platform::TextureFiltering::LINEAR);
    Platform::textureWrapSParameter(id, Platform::TextureWrap::REPEAT);
    Platform::textureWrapTParameter(id, Platform::TextureWrap::REPEAT);
    Platform::textureData(0, Platform::TextureFormat::RGBA, Platform::TextureFormat::RGBA, width, height, Platform::TextureType::UNSIGNED_BYTE, nullptr);
    Platform::textureBind(0);
    textures.insert(std::make_pair(name, id));
    return id;
}

PlatformId Context::textureNew(const std::string& name, const std::string& filename) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "cannot load data from " << filename << std::endl;
        return 0;
    }

    PlatformId id = Platform::createTexture();
    Platform::textureBind(id);
    Platform::textureMinParameter(id, Platform::TextureFiltering::LINEAR);
    Platform::textureMagParameter(id, Platform::TextureFiltering::LINEAR);
    Platform::textureWrapSParameter(id, Platform::TextureWrap::REPEAT);
    Platform::textureWrapTParameter(id, Platform::TextureWrap::REPEAT);
    Platform::textureData(0,
        Platform::TextureFormat::RGBA,
        channels == 4 ? Platform::TextureFormat::RGBA : Platform::TextureFormat::RGB,
        width, height, 
        Platform::TextureType::UNSIGNED_BYTE, data);
    Platform::textureGenMipmap();
    Platform::textureBind(0);

    stbi_image_free(data);
    textures.insert(std::make_pair(name, id));
    return id;
}

void Context::textureDestroy(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        Platform::destroyTexture(it->second);
        textures.erase(it);
    }
}

void Context::textureUse(const std::string& name, unsigned char slot) const {
    auto it = textures.find(name);
    if (it == textures.end()) {
        return;
    }
    
    Platform::textureActiveUnit(slot);
    Platform::textureBind(it->second);
}