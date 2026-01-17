#ifndef RENDERER_H
#define RENDERER_H

#include "System.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Renderer{
    private:
        int width;
        int height;
        Vector3* camera;
    public:
        Renderer(int width, int height);
        void run();
        void processInput(GLFWwindow* window);
        void drawSphere();
};

#endif