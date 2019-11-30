#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>

GLenum glCheckError_(const char*, int);

#define glCheckError() glCheckError_(__FILE__, __LINE__)