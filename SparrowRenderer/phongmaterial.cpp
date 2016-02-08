#include "phongmaterial.h"
#include "texture.h"
#include "sparrowrenderer.h"
#include "glassert.h"
#include "shader.h"
#include <glm/ext.hpp>

void PhongMaterial::bindAttributes(Shader* myShader)
{
    if(SparrowRenderer::isModernOpenGLAvailable() && myShader != NULL)
    {
        // TODO store the attributes location (in the shader class maybe)
        myShader->bindFloat(myShader->getLocation("materialNs"), shininess);

        if(normal_map != NULL)
        {
            normal_map->bind(NORMAL_MAP);
            myShader->bindInteger(myShader->getLocation("normalMap"), NORMAL_MAP);
        }

        if(ambient_texture != NULL)
        {
            ambient_texture->bind(AMBIENT_TEXTURE);
            myShader->bindInteger(myShader->getLocation("ambientTexture"), AMBIENT_TEXTURE);
        }
        else
            myShader->bindVec3(myShader->getLocation("materialKa"), ambient);

        if(diffuse_texture != NULL)
        {
            diffuse_texture->bind(DIFFUSE_TEXTURE);
            myShader->bindInteger(myShader->getLocation("diffuseTexture"), DIFFUSE_TEXTURE);
        }
        else
            myShader->bindVec3(myShader->getLocation("materialKd"), diffuse);

        if(specular_texture != NULL)
        {
            specular_texture->bind(SPECULAR_TEXTURE);
            myShader->bindInteger(myShader->getLocation("specularTexture"), SPECULAR_TEXTURE);
        }
        else
            myShader->bindVec3(myShader->getLocation("materialKs"), specular);

        if(alpha_mask != NULL)
        {
            alpha_mask->bind(ALPHA_MASK);
            myShader->bindInteger(myShader->getLocation("alphaMask"), ALPHA_MASK);
        }
    }
    else
    {
        // Crappy rendering code
        glAssert(glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(glm::vec4(ambient, 1))));
        glAssert(glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, glm::value_ptr(glm::vec4(diffuse, 1))));
        glAssert(glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(glm::vec4(specular, 1))));
        glAssert(glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess));
        if(diffuse_texture != NULL)
            diffuse_texture->bind(DIFFUSE_TEXTURE);
        else
        {
            GLenum texSlot = GL_TEXTURE0+DIFFUSE_TEXTURE;
            glAssert(glActiveTexture(texSlot));
            glAssert(glBindTexture(GL_TEXTURE_2D, 0));
        }
    }
}

unsigned int PhongMaterial::getFlags()
{
    unsigned int flags = 0;
    if(normal_map != NULL)
        flags |= NORMAL_MAP_FLAG;
    if(ambient_texture != NULL)
        flags |= AMBIENT_TEXTURE_FLAG;
    if(diffuse_texture != NULL)
        flags |= DIFFUSE_TEXTURE_FLAG;
    if(specular_texture != NULL)
        flags |= SPECULAR_TEXTURE_FLAG;
    if(alpha_mask != NULL)
        flags |= ALPHA_MASK_FLAG;
    return flags;
}

void PhongMaterial::setAmbientTexture(Texture* myTexture)
{
    ambient_texture = myTexture;
}

void PhongMaterial::setDiffuseTexture(Texture* myTexture)
{
    diffuse_texture = myTexture;
}

void PhongMaterial::setSpecularTexture(Texture* myTexture)
{
    specular_texture = myTexture;
}

void PhongMaterial::setNormalMap(Texture* myNormalMap)
{
    normal_map = myNormalMap;
}

void PhongMaterial::setAlphaMask(Texture* myAlphaMask)
{
    alpha_mask = myAlphaMask;
}
