#pragma once

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

#include "common.h"

class Window {
public:
    Window (int w, int h, char* title) {
        if (!glfwInit())
            throw std::runtime_error("GLFW init error");
        window = glfwCreateWindow(w, h, title, nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Window init error");
        }
        glfwMakeContextCurrent(window);
    }
    virtual ~Window() = default;

    virtual void run() {
        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            glViewport(0, 0, width, height);

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

            draw();

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
        glfwTerminate();
    }

    virtual void draw() {}
protected:
    GLFWwindow* window;
};
