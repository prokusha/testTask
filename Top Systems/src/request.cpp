#include "request.h"
#include "common.h"
#include "primitives.h"

void Request::draw(Requests::primitives primitive) {
    switch (primitive) {
        case Requests::primitives::TRIANGLE: {
            char title[] = "Triangle";
            window_ = new Triangle(Window_Settings::width, Window_Settings::height, title);
            break;
        }
        case Requests::primitives::SQUARE: {
            char title[] = "Square";
            window_ = new Square(Window_Settings::width, Window_Settings::height, title);
            break;
        }
        case Requests::primitives::RECTANGLE: {
            char title[] = "Rectangle";
            window_ = new Rectangle(Window_Settings::width, Window_Settings::height, title);
            break;
        }
        case Requests::primitives::CIRCLE: {
            char title[] = "Circle";
            window_ = new Circle(Window_Settings::width, Window_Settings::height, title);
            break;
        }
    }
    window_->run();
}
