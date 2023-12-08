#pragma once

#include <GL/glew.h>

struct Position {
    float x, y;
};

struct Color {
    GLubyte red, green, blue, alpha;
};

struct UV {
    float u, v;
};

struct Vertex {
    Position position;
    Color color;
    UV uv;

    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }

    void setColor(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {
        color.red = red;
        color.green = green;
        color.blue = blue;
        color.alpha = alpha;
    }

    void setUV(float u, float v) {
        uv.u = u;
        uv.v = v;
    }
};