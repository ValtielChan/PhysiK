#include "shadersource.h"
#include <string>
#include <vector>
#include <sstream>
#include "shader.h"

ShaderSource::ShaderSource()
{
    for(int i=0; i<NB_TYPES; ++i)
        sources[i] = NULL;
}

ShaderSource::~ShaderSource()
{
    for(int i=0; i<NB_TYPES; ++i)
        if(sources[i] != NULL)
            delete(sources[i]);
}

void ShaderSource::setSource(const char *source, SourceType type)
{
    if(sources[type] != NULL)
        delete(sources[type]);
    if(source == NULL)
        sources[type] = NULL;
    else
        sources[type] = new std::string(source);
}

Shader* ShaderSource::compile(int nbDefines, const char** defines)
{
    if(sources[VERTEX] == NULL || sources[FRAGMENT] == NULL)
        return NULL;
    std::string compiledSources[NB_TYPES];
    for(int i=0; i<NB_TYPES; ++i)
    {
        if(sources[i] == NULL)
            continue;
        compiledSources[i] = preprocess(*(sources[i]), nbDefines, defines);
    }
    if(sources[GEOMETRY] != NULL)
        return new Shader(compiledSources[VERTEX], compiledSources[GEOMETRY], compiledSources[FRAGMENT]);
    else
        return new Shader(compiledSources[VERTEX], compiledSources[FRAGMENT]);
}

bool isDefined(const std::string &str, int nbDefines, const char** defines)
{
    for(int i=0; i<nbDefines; ++i)
    {
        if(str.compare(defines[i]) == 0)
            return true;
    }
    return false;
}

std::string ShaderSource::preprocess(std::string source, int nbDefines, const char** defines)
{
    std::string compiled = "";
    std::istringstream ss(source);
    std::string line;
    std::vector<bool> allowedStack;
    while (std::getline(ss, line)) {
        if(line.size() > 0 && line.at(0) == '#')
        {
            if(line.compare(0, 8, "#version") == 0)
                compiled.append(line+'\n');
            else if(line.compare(0, 7, "#ifdef ") == 0)
                allowedStack.push_back(isDefined(line.substr(7), nbDefines, defines));
            else if(line.compare(0, 8, "#ifndef ") == 0)
                allowedStack.push_back(!isDefined(line.substr(8), nbDefines, defines));
            else if(line.compare("#endif") == 0)
                allowedStack.pop_back();
            else if(line.compare("#else") == 0)
                allowedStack.back() = !allowedStack.back();
        }
        else
        {
            bool ok = true;
            for(bool allowed : allowedStack)
            {
                if(!allowed)
                {
                    ok = false; break;
                }
            }
            if(ok)
                compiled.append(line+'\n');
        }
    }
    return compiled;
}
