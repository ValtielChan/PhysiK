#ifndef SHADER_H
#define SHADER_H

#include "glew.h"
#include <string>
#include <glm/fwd.hpp>

class Shader
{
    GLuint program;
    GLuint createShader(const std::string &source, GLenum shaderType);
    void printShaderInfoLog(GLuint shaderId);
    void printProgramInfoLog(GLuint programId);

public:
    Shader(const std::string &vertexSource, const std::string &fragmentSource);
    Shader(const std::string &vertexSource, const std::string &geometrySource, const std::string &fragmentSource);
    ~Shader();
    GLuint getLocation(std::string attribName);

    void bind();
    void unbind();
    void bindFloat(GLuint location, float val);
    void bindMat3(GLuint location, glm::mat3 mat);
    void bindMat4(GLuint location, glm::mat4 mat);
    void bindVec3(GLuint location, glm::vec3 vec);
    void bindVec4(GLuint location, glm::vec4 vec);
    void bindVec3Array(GLuint location, glm::vec3* vec, int nb_elements);
    void bindInteger(GLuint location, GLuint tex_id);
};

#endif // SHADER_H
