#include "glew.h"
#include "shader.h"
#include <iostream>
#include "glassert.h"
#include <glm/ext.hpp>

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource)
{
    program = glAssert(glCreateProgram());

    GLuint vertexShaderId = createShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShaderId = createShader(fragmentSource, GL_FRAGMENT_SHADER);

    glAssert(glAttachShader(program, vertexShaderId));
    glAssert(glAttachShader(program, fragmentShaderId));

    glAssert(glBindAttribLocation(program, 0, "inPosition"));
    glAssert(glBindAttribLocation(program, 1, "inNormal"));
    glAssert(glBindAttribLocation(program, 2, "inTexCoord"));

    glAssert(glLinkProgram(program));

    // check errors
    GLint linked;
    glAssert(glGetProgramiv(program, GL_LINK_STATUS, &linked));
    if (!linked) {
        std::cerr << "Program not linked" << std::endl;
        printProgramInfoLog(program);
        program = 0;
    }

    glAssert(glDetachShader(program, vertexShaderId));
    glAssert(glDetachShader(program, fragmentShaderId));
    glAssert(glDeleteShader(vertexShaderId));
    glAssert(glDeleteShader(fragmentShaderId));
}

Shader::Shader(const std::string &vertexSource, const std::string &geometrySource, const std::string &fragmentSource)
{
    program = glAssert(glCreateProgram());

    GLuint vertexShaderId = createShader(vertexSource, GL_VERTEX_SHADER);
    GLuint geometryShaderId = createShader(geometrySource, GL_GEOMETRY_SHADER);
    GLuint fragmentShaderId = createShader(fragmentSource, GL_FRAGMENT_SHADER);

    glAssert(glAttachShader(program, vertexShaderId));
    glAssert(glAttachShader(program, geometryShaderId));
    glAssert(glAttachShader(program, fragmentShaderId));

    glAssert(glBindAttribLocation(program, 0, "inPosition"));
    glAssert(glBindAttribLocation(program, 1, "inNormal"));
    glAssert(glBindAttribLocation(program, 2, "inTexCoord"));

    glAssert(glLinkProgram(program));

    // check errors
    GLint linked;
    glAssert(glGetProgramiv(program, GL_LINK_STATUS, &linked));
    if (!linked) {
        std::cerr << "Program not linked" << std::endl;
        printProgramInfoLog(program);
        program = 0;
    }

    glAssert(glDetachShader(program, vertexShaderId));
    glAssert(glDetachShader(program, geometryShaderId));
    glAssert(glDetachShader(program, fragmentShaderId));
    glAssert(glDeleteShader(vertexShaderId));
    glAssert(glDeleteShader(geometryShaderId));
    glAssert(glDeleteShader(fragmentShaderId));
}

Shader::~Shader()
{
    bool ok;
    glAssert(ok = glIsProgram(program))
    if(ok)
        glAssert(glDeleteProgram(program));
}

GLuint Shader::createShader(const std::string &source, GLenum shaderType)
{
    glAssert(GLuint shaderId = glCreateShader(shaderType));
    const GLchar *data = (const GLchar *)source.c_str();
    glAssert(glShaderSource(shaderId, 1, &data, NULL));

    glAssert(glCompileShader(shaderId));

    // check errors
    GLint compiled;
    glAssert(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled));
    if (!compiled) {
        std::string type_str;
        switch(shaderType)
        {
        case GL_VERTEX_SHADER:
            type_str = "Vertex";
            break;
        case GL_GEOMETRY_SHADER:
            type_str = "Geometry";
            break;
        case GL_FRAGMENT_SHADER:
            type_str = "Fragment";
            break;
        }
        std::cerr << type_str << " shader not compiled : " << std::endl;
        std::cout << "Shader source :" << std::endl << source << std::endl;
        printShaderInfoLog(shaderId);
        return 0;
    }
    else
        return shaderId;
}

void Shader::printShaderInfoLog(GLuint shaderId)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;
    glAssert(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLen));
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        glAssert(glGetShaderInfoLog(shaderId, infoLogLen, &charsWritten, infoLog));
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete [] infoLog;
    }
}

void Shader::printProgramInfoLog(GLuint programId)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;
    glAssert(glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLen));
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        glAssert(glGetProgramInfoLog(programId, infoLogLen, &charsWritten, infoLog));
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete [] infoLog;
    }
}

GLuint Shader::getLocation(std::string attribName)
{
    glAssert(GLuint loc = glGetUniformLocation(program, attribName.c_str()));
    return loc;
}

void Shader::bind()
{
    glAssert(glUseProgram(program));
}

void Shader::unbind()
{
    glAssert(glUseProgram(0));
}

void Shader::bindFloat(GLuint location, float val)
{
    glAssert(glUniform1f(location, val));
}

void Shader::bindMat3(GLuint location, glm::mat3 mat)
{
    glAssert(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
}

void Shader::bindMat4(GLuint location, glm::mat4 mat)
{
    glAssert(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
}

void Shader::bindVec3(GLuint location, glm::vec3 vec)
{
    glAssert(glUniform3fv(location, 1, glm::value_ptr(vec)));
}

void Shader::bindVec4(GLuint location, glm::vec4 vec)
{
    glAssert(glUniform4fv(location, 1, glm::value_ptr(vec)));
}

void Shader::bindVec3Array(GLuint location, glm::vec3* vec, int nb_elements)
{
    glAssert(glUniform3fv(location, nb_elements, (GLfloat*)vec));
}

void Shader::bindUnsignedInteger(GLuint location, GLuint unsigned_integer)
{
    glAssert(glUniform1ui(location, unsigned_integer));
}

void Shader::bindInteger(GLuint location, GLint integer)
{
    glAssert(glUniform1i(location, integer));
}
