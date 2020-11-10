#include "window.hpp"

int main(void)
{
    createWindow(1280, 900);
    while (windowIsOpen()) {}
    destroyWindow();
    return 0;
}