#ifndef RENDERER_H
#define RENDERER_H

#include "Particle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Renderer{
    private:
        int width;
        int height;
        Vector3* camera;
    public:
        Renderer(int width, int height, Vector3* camera);
        void run();
        void drawSphere();
};

#endif