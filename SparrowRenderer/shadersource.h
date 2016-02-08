#ifndef SHADERSOURCE_H
#define SHADERSOURCE_H

#include <string>

class Shader;

class ShaderSource
{
public:
    enum SourceType
    {
        VERTEX,
        GEOMETRY,
        FRAGMENT,
        NB_TYPES
    };

    ShaderSource();
    ~ShaderSource();

    void setSource(const char *source, SourceType type);

    Shader* compile(int nbDefines = 0, const char** defines = NULL);

private:
    std::string* sources[NB_TYPES];

    std::string preprocess(std::string source, int nbDefines, const char** defines);
};

#endif // SHADERSOURCE_H
