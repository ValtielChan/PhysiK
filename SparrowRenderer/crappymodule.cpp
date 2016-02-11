#include "crappymodule.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "sparrowrenderer.h"
#include "scene.h"
#include "glassert.h"
#include "material.h"
#include <glm/ext.hpp>

void CrappyModule::renderGL(Camera* myCamera, Scene* scene)
{
    glAssert(glEnable(GL_LIGHTING));
    glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    glLoadIdentity();
    glAssert(glEnable(GL_LIGHT0));
    glAssert(glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(glm::vec4(glm::vec3(0.1f), 1))));
    glAssert(glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(glm::vec4(0))));
    glAssert(glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(glm::vec4(0, 0, 0, 1))));
    glAssert(glLightfv(GL_LIGHT0, GL_SPECULAR, glm::value_ptr(glm::vec4(0, 0, 0, 1))));

    int i=1;
    for(SceneIterator<Light*>* lightIt = scene->getLights();
        lightIt->isValid(); lightIt->next())
    {
        Light* l = lightIt->getItem();
        glAssert(glEnable(GL_LIGHT0 + i));
        glAssert(glLightfv(GL_LIGHT0 + i, GL_AMBIENT, glm::value_ptr(glm::vec4(glm::vec3(0), 1))));
        if(l->isDirectionnal()){
            glAssert(glLightfv(GL_LIGHT0 + i, GL_POSITION, glm::value_ptr(glm::vec4(-l->getDir(), 0))));
        }else{
            glAssert(glLightfv(GL_LIGHT0 + i, GL_POSITION, glm::value_ptr(glm::vec4(l->getPos(), 1))));
        }
        glAssert(glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, glm::value_ptr(glm::vec4(l->getColor(), 1))));
        glAssert(glLightfv(GL_LIGHT0 + i, GL_SPECULAR, glm::value_ptr(glm::vec4(l->getColor(), 1))));
        ++i;
    }

    for(SceneIterator<GeometryNode*>* geometryIt = scene->getGeometry();
        geometryIt->isValid(); geometryIt->next())
    {
        GeometryNode* node = geometryIt->getItem();
        glMatrixMode(GL_MODELVIEW);
        glm::mat4 modelViewMatrix = myCamera->getViewMatrix() * node->modelMatrix;
        glLoadMatrixf(glm::value_ptr(modelViewMatrix));
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(myCamera->getProjectionMatrix()));
        node->mesh->draw();
    }

    glAssert(glDisable(GL_LIGHTING));
    for(int j=0; j<i; ++j)
        glAssert(glDisable(GL_LIGHTING));
}
