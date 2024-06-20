#pragma once

#include "window.h"
#include "common.h"

class Request {
public:
    void draw(Requests::primitives primitive);
private:
    Window * window_;
};
