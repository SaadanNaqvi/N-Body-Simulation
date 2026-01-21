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
        const float visualScale = 0.5e-9f;
        const float radiusScale = 2e-8;
        double minSphere = 1.0;    
        double maxSphere = 5.00;


        // Sphere stuff
        const double PI = 3.14159265359;

        unsigned int sphereVAO = 0;
        unsigned int sphereVBO = 0;
        unsigned int sphereEBO = 0;
        unsigned int indexCount = 0;


        


        // Trail
        std::unordered_map<Particle*, std::deque<Vector3>> trails;    
        int maxTrailPoints = 500;
        unsigned int trailVAO = 0;
        unsigned int trailVBO = 0;

    public:
        Renderer(int width, int height);
        void run(System& system);
        void processInput(GLFWwindow* window, Camera& camera, float deltaTime, System& system);
        void generateSphere(double radius, int sectors, int stackCount, std::vector<Vector3>& vertices, std::vector<unsigned int>& indices);
        void initSphere();
        void drawSphere(std::vector<Particle*>& particle, Shader& shader);
        void initTrails();
        void updateTrails(std::vector<Particle*>& particles);
        void drawTrails(std::vector<Particle*>& particles, Shader& trailShader, glm::mat4& view, glm::mat4& projection);
        void clearTrails();
};

#endif