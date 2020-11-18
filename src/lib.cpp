#include "lib.hpp"
#include <cassert>

AY_LIB Window* windowNew(int width, int height) {
    return new Window(width, height);
}

AY_LIB void windowDispose(const Window* window) {
    assert(window);
    delete window;
}

AY_LIB bool windowIsOpen(Window* window) {
    assert(window);
    return window->isOpen();
}

AY_LIB Context* windowGetContext(const Window* window) {
    assert(window);
    return window->getContext();
}