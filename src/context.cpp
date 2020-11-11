#include "context.hpp"
#include "math.hpp"
#include <iostream>
#include <fstream>
#include <stb_image.h>

Context::~Context() {
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        OpenGL::destroyProgram(it->second);
    }

    for (auto it = textures.begin(); it != textures.end(); it++) {
        OpenGL::destroyTexture(it->second);
    }
}

void Context::clear(float r, float g, float b, float a) const {
    OpenGL::clear(r, g, b, a);
}

PlatformId Context::shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment) {
    PlatformId id = OpenGL::createProgram();
    PlatformId vid = OpenGL::createVertexShader();
    PlatformId fid = OpenGL::createFragmentShader();
    std::string log = "";
    
    OpenGL::shaderSource(vid, vertex);
    OpenGL::compileShader(vid);
    log = OpenGL::getShaderLog(vid);
    if (log.size() > 0) {
        std::cerr << "Vertex: " << log << std::endl;
        return 0;
    }

    OpenGL::shaderSource(fid, fragment);
    OpenGL::compileShader(fid);
    log = OpenGL::getShaderLog(fid);
    if (log.size() > 0) {
        std::cerr << "Fragment: " << log << std::endl;
        return 0;
    }

    OpenGL::attachShader(id, vid);
    OpenGL::attachShader(id, fid);
    OpenGL::linkProgram(id);
    log = OpenGL::getProgramLog(id);
    if (log.size() > 0) {
        std::cerr << "Program: " << log << std::endl;
        return 0;
    }
    OpenGL::detachShader(id, vid);
    OpenGL::detachShader(id, fid);
    OpenGL::destroyShader(vid);
    OpenGL::destroyShader(fid);
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
        OpenGL::destroyShader(it->second);
        shaders.erase(it);
    }
}

void Context::shaderUse(const std::string& name) {
    auto shader = shaders.find(name);
    if (shader != shaders.end()) {
        currentShader = shader->second;
    }
    OpenGL::useProgram(currentShader);
}

void Context::shaderUniform(const std::string& name, float value) const {
    OpenGL::uniform1f(currentShader, name, value);
}

void Context::shaderUniform(const std::string& name, int value) const {
    OpenGL::uniform1i(currentShader, name, value);
}

void Context::shaderUniform(const std::string& name, const Vec3& value) const {
    OpenGL::uniform3fv(currentShader, name, new float[3] { value.x, value.y, value.z });
}

PlatformId Context::vaoNew() const {
    return OpenGL::createVertexArray();
}

void Context::vaoDestroy(PlatformId id) const {
    OpenGL::destroyVertexArray(id);
}

void Context::vaoBind(PlatformId id) const {
    OpenGL::bindVertexArray(id);
}

PlatformId Context::bufferNew() const {
    return OpenGL::createBuffer();
}

void Context::bufferDestroy(PlatformId id) const {
    OpenGL::destroyBuffer(id);
}

void Context::bufferBind(PlatformId id, OpenGL::BufferMode mode) const {
    OpenGL::bindBuffer(id, mode);
}

void Context::bufferData(OpenGL::BufferMode mode, size_t size, const void* data, OpenGL::BufferTarget target) const {
    OpenGL::bufferData(mode, size, data, target);
}

void Context::bufferAttribArray(PlatformId id, int size, OpenGL::AttribType type, size_t stride, const void* ptr) {
    OpenGL::vertexAttribArray(id, size, type, stride, ptr);
}

void Context::drawArrays(OpenGL::DrawMode mode, int first, size_t count) const {
    OpenGL::drawArrays(mode, first, count);
}

void Context::drawElements(OpenGL::DrawMode mode, size_t count, OpenGL::AttribType type, const void* indices) {
    OpenGL::drawElements(mode, count, type, indices);
}

PlatformId Context::textureNew(const std::string& name, int width, int height) {
    PlatformId id = OpenGL::createTexture();
    OpenGL::textureBind(id);
    OpenGL::textureMinParameter(id, OpenGL::TextureFiltering::LINEAR);
    OpenGL::textureMagParameter(id, OpenGL::TextureFiltering::LINEAR);
    OpenGL::textureWrapSParameter(id, OpenGL::TextureWrap::REPEAT);
    OpenGL::textureWrapTParameter(id, OpenGL::TextureWrap::REPEAT);
    OpenGL::textureData(0, OpenGL::TextureFormat::RGBA, OpenGL::TextureFormat::RGBA, width, height, OpenGL::TextureType::UNSIGNED_BYTE, nullptr);
    OpenGL::textureBind(0);
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

    PlatformId id = OpenGL::createTexture();
    OpenGL::textureBind(id);
    OpenGL::textureMinParameter(id, OpenGL::TextureFiltering::LINEAR);
    OpenGL::textureMagParameter(id, OpenGL::TextureFiltering::LINEAR);
    OpenGL::textureWrapSParameter(id, OpenGL::TextureWrap::REPEAT);
    OpenGL::textureWrapTParameter(id, OpenGL::TextureWrap::REPEAT);
    OpenGL::textureData(0, 
        OpenGL::TextureFormat::RGBA, 
        channels == 4 ? OpenGL::TextureFormat::RGBA : OpenGL::TextureFormat::RGB, 
        width, height, 
        OpenGL::TextureType::UNSIGNED_BYTE, data);
    OpenGL::textureGenMipmap();
    OpenGL::textureBind(0);

    stbi_image_free(data);
    textures.insert(std::make_pair(name, id));
    return id;
}

void Context::textureDestroy(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        OpenGL::destroyTexture(it->second);
        textures.erase(it);
    }
}

void Context::textureUse(const std::string& name, unsigned char slot) const {
    auto it = textures.find(name);
    if (it == textures.end()) {
        return;
    }
    
    OpenGL::textureActiveUnit(slot);
    OpenGL::textureBind(it->second);
}