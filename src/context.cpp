#include "context.hpp"

Context::~Context() {
    for (auto shader : shaders) {
        OpenGL::destroyProgram(shader);
    }
}