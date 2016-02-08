#include "light.h"
#include "framebuffer.h"
#include "texture.h"
#include "scene.h"
#include "shader.h"
#include "shadersource.h"
#include "phongmaterial.h"
#include "mesh.h"
#include "glassert.h"
#include <glm/ext.hpp>

const char* Light::flagStr[] = {
    "AMBIENT_LIGHT",
    "DIRECTIONNAL_LIGHT",
    "POINT_LIGHT",
    "SPOT_LIGHT",
    "SHADOWMAP"
};

const glm::mat4 Light::biasMatrix(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0);

Light::Light()
{
    initPointLight();
}

void Light::initDirectionnalLight(glm::vec3 dir, glm::vec3 lightColor)
{
    type = DIRECTIONNAL;
    position = glm::vec3(0);
    direction = dir;
    color = lightColor;
    shadowCaster = false;
    isDir = true;
    viewMatrix = glm::lookAt(position, position+direction, glm::vec3(0, 1, 0));
}

void Light::initPointLight(glm::vec3 pos, glm::vec3 lightColor, float att)
{
    type = POINT;
    position = pos;
    cutOffAngle = 360;
    color = lightColor;
    shadowCaster = false;
    isDir = false;
    viewMatrix = glm::mat4();
    attenuation = att;
}

void Light::initSpotLight(glm::vec3 pos, glm::vec3 dir, float spotAngle, glm::vec3 lightColor)
{
    type = SPOT;
    position = pos;
    direction = dir;
    cutOffAngle = spotAngle;
    color = lightColor;
    shadowCaster = false;
    isDir = false;
}

bool Light::isDirectionnal()
{
    return isDir;
}

void Light::initShadowMap(int resWidth, int resHeight, glm::vec3 dim)
{
    shadowMapWidth = resWidth;
    shadowMapHeight = resHeight;
    viewMatrix = glm::lookAt(position, position+direction, glm::vec3(0, 1, 0));
    if(type == DIRECTIONNAL)
        projectionMatrix = glm::ortho(-dim.x/2, dim.x/2, -dim.y/2, dim.y/2, -dim.z/2, dim.z/2);
    else
        ; // TODO : glm::projection

    shadowCaster = true;
    shadowMap = new FrameBuffer();
    // Depth buffer
    Texture* tex = new Texture(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, resWidth, resHeight, GL_FLOAT);
    tex->setFiltering(GL_LINEAR);
    tex->setWrap(GL_CLAMP_TO_EDGE);
    shadowMap->addTexture(tex, GL_DEPTH_ATTACHMENT);
    shadowMap->initColorAttachments();

    ShaderSource source;
    source.setSource(vertSource, ShaderSource::VERTEX);
    source.setSource(fragSource, ShaderSource::FRAGMENT);
    std::vector<const char*> defines;
    shaders[0] = source.compile(defines.size(), defines.data());
    defines.push_back("ALPHA_MASK");
    shaders[1] = source.compile(defines.size(), defines.data());
}

void Light::generateShadowMap(Scene* scene)
{
    glAssert(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    glAssert(glViewport(0, 0, shadowMapWidth, shadowMapHeight));
    shadowMap->bindFBO();
    glAssert(glClearDepth(1.0));
    glAssert(glClear(GL_DEPTH_BUFFER_BIT));
    glAssert(glEnable(GL_DEPTH_TEST));
    glAssert(glDepthFunc(GL_LESS));
    glAssert(glCullFace(GL_FRONT));
    for(SceneIterator<GeometryNode*>* geometryIt = scene->getGeometry();
        geometryIt->isValid(); geometryIt->next())
    {
        GeometryNode* node = geometryIt->getItem();
        Material* mat = node->mesh->material;
        if(((PhongMaterial*)mat)->castShadow)
        {
            // compute matrix attributes
            glm::mat4 lightMVP = getProjectionMatrix() * (getViewMatrix() * node->modelMatrix);
            if(mat->getFlags() & ALPHA_MASK_FLAG)
            {
                PhongMaterial* pmat = (PhongMaterial*)mat;
                shaders[1]->bind();
                pmat->alpha_mask->bind(ALPHA_MASK);
                shaders[1]->bindMat4(shaders[1]->getLocation("MVP"), lightMVP);
                shaders[1]->bindInteger(shaders[1]->getLocation("alphaMask"), ALPHA_MASK);
                node->mesh->draw(shaders[1], false, true, false);
            }
            else
            {
                shaders[0]->bind();
                shaders[0]->bindMat4(shaders[1]->getLocation("MVP"), lightMVP);
                node->mesh->draw(shaders[0], false, false, false);
            }
        }
    }
    glAssert(glCullFace(GL_BACK));
}

Texture* Light::getShadowMap()
{
    return shadowMap->getTexture(0);
}

void Light::setPosition(glm::vec3 new_pos)
{
    position = new_pos;
    viewMatrix = glm::lookAt(position, position+direction, glm::vec3(0, 1, 0));
}

unsigned int Light::getFlags(Light* l)
{
    if(l == NULL)
        return 1 << AMBIENT_FLAG;
    else
    {
        unsigned int flags = 0;
        switch(l->getType())
        {
        case DIRECTIONNAL :
            flags |= 1 << DIRECTIONNAL_FLAG;
            break;
        case POINT :
            flags |= 1 << POINT_FLAG;
            break;
        case SPOT :
            flags |= 1 << SPOT_FLAG;
            break;
        }
        if(l->isShadowCaster())
            flags |= 1 << SHADOWMAP_FLAG;
        return flags;
    }
}

const char* Light::vertSource =
        "#version 330 core\n\
        layout(location = 0)in vec3 inPosition;\n\
        #ifdef ALPHA_MASK\n\
        layout(location = 2)in vec2 inTexCoord;\n\
        out vec3 varTexCoord;\n\
        #endif\n\
        uniform mat4 MVP;\n\
        void main()\n\
        {\n\
        #ifdef ALPHA_MASK\n\
            varTexCoord = inTexCoord.xy;\n\
        #endif\n\
            gl_Position = MVP * vec4(inPosition, 1.0);\n\
        }\n";

const char* Light::fragSource =
        "#version 330 core\n\
        #ifdef ALPHA_MASK\n\
        uniform sampler2D alphaMask;\n\
        in vec3 varTexCoord;\n\
        #endif\n\
        out float fragmentdepth;\n\
        void main()\n\
        {\n\
            #ifdef ALPHA_MASK\n\
                if(texture(alphaMask, varTexCoord).r < 0.5)\n\
                    discard;\n\
            #endif\n\
            fragmentdepth = gl_FragCoord.z;\n\
        }\n";
