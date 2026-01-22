#include "Shader.h"

Shader::Shader(const char *vs, const char *fs)
{
    int v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs, nullptr);
    glCompileShader(v);

    int f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs, nullptr);
    glCompileShader(f);

    ID = glCreateProgram();
    glAttachShader(ID, v);
    glAttachShader(ID, f);
    glLinkProgram(ID);

    glDeleteShader(v);
    glDeleteShader(f);
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setMat4(std::string name, glm::mat4 &mat){
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}


void Shader::setVec3(const std::string& name, const glm::vec3& v){
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
}

void Shader::setFloat(const std::string& name, const float v){
    glUniform1f(glGetUniformLocation(ID, name.c_str()), v);
}