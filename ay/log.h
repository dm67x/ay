#pragma once

#include "ay.h"

#include <glad/glad.h>
#include <string>
#include <iostream>

AY_API GLenum glCheckError_(const char*, int);

#define glCheckError() glCheckError_(__FILE__, __LINE__)