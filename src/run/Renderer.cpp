#include "Renderer.h"



Renderer::Renderer(int width, int height){
    this->width = width;
    this->height = height;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Renderer::run(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(width, height, "N Body Simulation", NULL, NULL);
    if(window == nullptr){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return;
    }

    
    glEnable(GL_DEPTH_TEST);
    initSphere();

    // Render loop
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        drawSphere();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();

}

void Renderer::processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}



void Renderer::generateSphere(double radius, int sectors, int stackCount, std::vector<Vector3>& vertices, std::vector<unsigned int>& indices){
    for(int i = 0; i<= stackCount; i++){
        double stackAngle = PI / 2 - i * PI / stackCount;
        double xy = radius * cosf(stackAngle);
        double z = radius * sinf(stackAngle);

        for(int j = 0; j <= sectors; ++j) {
            double sectorAngle = j * 2 * PI / sectors;
            Vector3 v(xy * cosf(sectorAngle), xy * sinf(sectorAngle), z);

            vertices.push_back(v);
        }
    }

    for (int i = 0; i < stackCount; i++) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; j++, k1++, k2++) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Renderer::drawSphere(){
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void Renderer::initSphere(){
    std::vector<Vector3> vertices;
    std::vector<unsigned int> indices;

    generateSphere(1.0, 36, 18, vertices, indices);
    indexCount = indices.size();

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
