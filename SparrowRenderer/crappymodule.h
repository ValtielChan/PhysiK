#ifndef CRAPPYMODULE_H
#define CRAPPYMODULE_H

#include "module.h"
#include <cstddef>
#include "glew.h"

class CrappyModule : public Module
{
public:
    void renderGL(Camera* myCamera, Scene* scene = NULL);
    bool requiresModernOpenGL() {return false;}
};

#endif // CRAPPYMODULE_H
