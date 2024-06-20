#include <GLFW/glfw3.h>
#include <iostream>

#include "request.h"
#include "common.h"

int main(void)
{
    Request reque;

    while (true) {
        std::cout << "Options:\n"
                     "1) Draw triangle\n"
                     "2) Draw square\n"
                     "3) Draw rectagle\n"
                     "4) Draw circle\n"
                     "5) Exit\n";
        std::cout << "Command: ";
        int i;
        std::cin >> i;
        Requests::primitives primitive;
        switch (i) {
            case 1: {
                primitive = Requests::primitives::TRIANGLE;
                break;
            }
            case 2: {
                primitive = Requests::primitives::SQUARE;
                break;
            }
            case 3: {
                primitive = Requests::primitives::RECTANGLE;
                break;
            }
            case 4: {
                primitive = Requests::primitives::CIRCLE;
                break;
            }
            case 5: {
                return 0;
                break;
            }
        }
        std::cout << "\nWindow opening\n";
        reque.draw(primitive);
    }

    glfwTerminate();
    return 0;
}
