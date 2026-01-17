#include "Renderer.h"


Renderer::Renderer(int width, int height, Vector3* camera){
    this->width = width;
    this->height = height;
    this->camera = camera;
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

}


void Renderer::drawSphere(){

}