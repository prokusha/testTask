#pragma once

#include "window.h"

class Triangle : public Window {
public:
    Triangle(int w, int h, char* title) : Window(w, h, title) {}
    void draw() override;
};

class Square : public Window {
public:
    Square(int w, int h, char* title) : Window(w, h, title) {}
    void draw() override;
};

class Rectangle : public Window {
public:
    Rectangle(int w, int h, char* title) : Window(w, h, title) {}
    void draw() override;
};

class Circle : public Window {
public:
    Circle(int w, int h, char* title) : Window(w, h, title) {}
    void draw() override;
};
