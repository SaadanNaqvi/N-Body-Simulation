#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
    private:
        int ID;
    public:
        Shader(const char *vs, const char *fs);

        void use();
        void setMat4(std::string name, glm::mat4 &mat);
};

#endif
