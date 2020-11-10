#include "context.hpp"
#include <iostream>

Context::~Context() {
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        OpenGL::destroyProgram(it->second);
    }
}

PlatformId Context::newShaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment) {
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

void Context::useShader(const std::string& name) {
    auto shader = shaders.find(name);
    if (shader != shaders.end()) {
        currentShader = shader->second;
    }
    OpenGL::useProgram(currentShader);
}