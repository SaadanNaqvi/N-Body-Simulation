#ifndef RENDERER_H
#define RENDERER_H

#include "System.h"
#include "Shader.h"
#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Renderer{
    private:
        int width;
        int height;


        const double PI = 3.14159265359;

        unsigned int sphereVAO = 0;
        unsigned int sphereVBO = 0;
        unsigned int sphereEBO = 0;
        unsigned int indexCount = 0;

        const float visualScale = 1e-9f;

    public:
        Renderer(int width, int height);
        void run(System& system);
        void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
        void generateSphere(double radius, int sectors, int stackCount, std::vector<Vector3>& vertices, std::vector<unsigned int>& indices);
        void initSphere();
        void drawSphere(std::vector<Particle*>& particle, Shader& shader);
};

#endif