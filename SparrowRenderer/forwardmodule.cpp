#include "forwardmodule.h"
#include "scene.h"
#include "mesh.h"
#include "shader.h"
#include "light.h"
#include "glassert.h"
#include "texture.h"
#include <glm/ext.hpp>

const char* const ForwardModule::flagStr[] =
{
    "DIFFUSE_TEXTURE",
    "AMBIENT_TEXTURE",
    "SPECULAR_TEXTURE",
    "NORMAL_MAP",
    "ALPHA_MASK",
    "INSTANCING"
};

void ForwardModule::renderGL(Camera* myCamera, Scene* scene)
{
    // bind target
    renderTarget->bindFBO();

    // render ambient lighting
    glAssert(glDepthFunc(GL_LESS));
    glAssert(glDisable(GL_BLEND));

    lightPass(myCamera, scene, NULL);

    // render directionnal lighting and point lighting
    glAssert(glDepthFunc(GL_LEQUAL));
    glAssert(glEnable(GL_BLEND));
    glAssert(glBlendFunc(GL_ONE, GL_ONE));
    glAssert(glDepthMask(GL_FALSE));

    for(SceneIterator<Light*>* lightIt = scene->getLights(); lightIt->isValid(); lightIt->next())
        lightPass(myCamera, scene, lightIt->getItem());

    glAssert(glDisable(GL_BLEND));
    glAssert(glDepthFunc(GL_LESS));
    glAssert(glDepthMask(GL_TRUE));
}

void ForwardModule::lightPass(Camera* myCamera, Scene* scene, Light* light)
{
    if(isWireframe){
        glAssert(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }else{
        glAssert(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }
    // loop over all types of geometry
    for(int i=0; i<geometryFlagList.size(); ++i)
    {
        int j;
        for(j=0; j<lightFlagList.size(); ++j)
            if(lightFlagList[j] == Light::getFlags(light))
                break;
        if(j == lightFlagList.size())
            continue; // WARNING : missing shader for the light
        Shader* shader = shaders[i*lightFlagList.size() + j];
        shader->bind();

        // bind light attributes
        if(light == NULL)
        {
            // ambient light
            shader->bindVec3(shader->getLocation("lightColor"), glm::vec3(0.1f)); // add attribute, and setter for ambient lighting
        }
        else
        {
            switch(light->getType())
            {
            case Light::DIRECTIONNAL:
                shader->bindVec3(shader->getLocation("dirLight"), -light->getDir());
                shader->bindVec3(shader->getLocation("lightColor"), light->getColor());
                if(light->isShadowCaster())
                {
                    light->getShadowMap()->bind(NB_FLAGS); // NB_FLAGS has the value of the first available slot after the phong material texture slots
                    shader->bindInteger(shader->getLocation("shadowMap"), NB_FLAGS);
                }
                break;
            case Light::POINT:
                shader->bindVec3(shader->getLocation("pointLight"), light->getPos());
                shader->bindVec3(shader->getLocation("lightColor"), light->getColor());
                shader->bindFloat(shader->getLocation("attenuation"), light->getAttenuation());
                break;
            case Light::SPOT:
                shader->bindVec3(shader->getLocation("lightColor"), light->getColor());
                // TODO add cutoff and attenuation
            break;
            }
        }
        for(SceneIterator<GeometryNode*>* geometryIt = scene->getGeometry();
            geometryIt->isValid(); geometryIt->next())
        {
            GeometryNode* node = geometryIt->getItem();
            Material* mat = node->mesh->material;
            if(mat->getFlags() == geometryFlagList[i]) // if flag matches material
            {
                // compute matrix attributes
                glm::mat4 modelViewMatrix = myCamera->getViewMatrix() * node->modelMatrix;
                glm::mat4 mvp = myCamera->getProjectionMatrix() * modelViewMatrix;
                glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));
                if(light != NULL && light->isShadowCaster())
                {
                    glm::mat4 lightMVP = Light::biasMatrix * light->getProjectionMatrix() * light->getViewMatrix() * node->modelMatrix;
                    shader->bindMat4(shader->getLocation("lightMVP"), lightMVP);
                }
                shader->bindMat4(shader->getLocation("viewMatrix"), myCamera->getViewMatrix());
                shader->bindMat4(shader->getLocation("modelViewMatrix"), modelViewMatrix);
                shader->bindMat3(shader->getLocation("normalMatrix"), glm::mat3(normalMatrix));
                shader->bindMat4(shader->getLocation("MVP"), mvp);
                node->mesh->draw(shader);
            }
        }
    }
}

// modern opengl methods

void ForwardModule::setShaderSource(ShaderSource* source)
{
    if(shaderSources != NULL)
        delete(shaderSources);
    shaderSources = source;
}

void ForwardModule::compileShaders(Scene* scene)
{
    geometryFlagList.clear();
    lightFlagList.clear();
    for(Shader* s : shaders)
        delete(s);
    shaders.clear();

    // get material flags
    const int nb_geometry_flags = 1 << NB_FLAGS;
    bool geometryFlags[nb_geometry_flags];
    for(int i=0; i<nb_geometry_flags; ++i)
        geometryFlags[i] = false;

    for(SceneIterator<GeometryNode*>* geometryIt = scene->getGeometry();
        geometryIt->isValid(); geometryIt->next())
    {
        Mesh* m = geometryIt->getItem()->mesh;
        unsigned int flags = m->material->getFlags();
        if(m->hasInstances())
            flags |= INSTANCING_FLAG;
        geometryFlags[flags] = true;
    }
    for(int i=0; i<nb_geometry_flags; ++i)
    {
        if(geometryFlags[i])
            geometryFlagList.push_back(i);
    }

    // get light flags
    const int nb_light_flags = 1 << Light::NB_LIGHT_FLAGS;
    bool lightFlags[nb_light_flags];
    for(int i=0; i<nb_light_flags; ++i)
        lightFlags[i] = false;

    // ambient light
    lightFlags[Light::getFlags(NULL)] = true;
    // scene lights
    for(SceneIterator<Light*>* lightIt = scene->getLights();
        lightIt->isValid(); lightIt->next())
    {
        Light* l = lightIt->getItem();
        lightFlags[Light::getFlags(l)] = true;
    }
    for(int i=0; i<nb_light_flags; ++i)
    {
        if(lightFlags[i])
            lightFlagList.push_back(i);
    }

    // shader compilation
    for(int i : geometryFlagList)
    {
        std::vector<const char*> defines;

        // set geometry defines
        if(i & NORMAL_MAP_FLAG)
            defines.push_back(flagStr[NORMAL_MAP]);
        if(i & AMBIENT_TEXTURE_FLAG)
            defines.push_back(flagStr[AMBIENT_TEXTURE]);
        if(i & DIFFUSE_TEXTURE_FLAG)
            defines.push_back(flagStr[DIFFUSE_TEXTURE]);
        if(i & SPECULAR_TEXTURE_FLAG)
            defines.push_back(flagStr[SPECULAR_TEXTURE]);
        if(i & ALPHA_MASK_FLAG)
            defines.push_back(flagStr[ALPHA_MASK]);
        if(i & INSTANCING_FLAG)
            defines.push_back(flagStr[INSTANCING]);

        int boundary = defines.size();
        for(int j : lightFlagList)
        {
            while(defines.size() > boundary)
                defines.pop_back();
            // set light defines
            for(int k=0; k<Light::NB_LIGHT_FLAGS; ++k)
            {
                if(j & (1 << k))
                    defines.push_back(Light::flagStr[k]);
            }
            // compilation
            shaders.push_back(shaderSources->compile(defines.size(), defines.data()));
        }
    }
}

void ForwardModule::setRenderTarget(const FrameBuffer* target)
{
    if(target != NULL)
        renderTarget = target;
}
