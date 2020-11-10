#include "window.hpp"
#include "context.hpp"

int main(void)
{
    createWindow(1280, 900);
    Context* ctx = new Context();

    while (windowIsOpen()) {

    }

    delete ctx;
    destroyWindow();
    return 0;
}