#include "window.h"

int main(void)
{
    ayContext context = { 3, 3 };
    ayWindow window = ayWindow_new(1280, 900, &context);
    while (ayWindow_show(&window)) {

    }
    ayWindow_destroy(&window);
    return 0;
}