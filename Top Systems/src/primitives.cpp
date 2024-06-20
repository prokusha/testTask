#include "primitives.h"

#include <GL/gl.h>
#include <cmath>

void Triangle::draw() {
    glBegin(GL_TRIANGLES);
    glColor3f(0.13f, 0.14f, 0.18f);

    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, -0.5f);

    glEnd();
}

void Square::draw() {
    glBegin(GL_QUADS);
    glColor3f(0.13f, 0.14f, 0.18f);

    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);

    glEnd();
}

void Rectangle::draw() {
    glBegin(GL_QUADS);
    glColor3f(0.13f, 0.14f, 0.18f);

    glVertex2f(-0.75f, -0.45f);
    glVertex2f(-0.75f, 0.45f);
    glVertex2f(0.75f, 0.45f);
    glVertex2f(0.75f, -0.45f);

    glEnd();
}

void Circle::draw() {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.13f, 0.14f, 0.18f);
    glVertex2f(0, 0);
    for(float i = 0.0f; i <= 360; i++)
        glVertex2f(0.5f*cos(M_PI * i / 180.0), 0.5f*sin(M_PI * i / 180.0));

    glEnd();
}
