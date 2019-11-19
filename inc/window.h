#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "context.h"

typedef struct {
    uint32_t width;
    uint32_t height;
    ayContext* context;
    GLFWwindow* glwin;
} ayWindow;

ayWindow ayWindow_new(uint32_t, uint32_t, ayContext*);
void ayWindow_destroy(ayWindow*);
bool ayWindow_show(ayWindow*);