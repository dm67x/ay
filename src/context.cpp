#include "context.hpp"
#include "math.hpp"
#include <iostream>
#include <fstream>

Context::~Context() {
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        OpenGL::destroyProgram(it->second);
    }
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

void Context::drawArrays(OpenGL::DrawMode mode, int first, size_t count) const {
    OpenGL::drawArrays(mode, first, count);
}