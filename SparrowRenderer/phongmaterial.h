#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "material.h"
#include "glm/vec3.hpp"

class Texture;

class PhongMaterial : public Material
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    bool castShadow;
    Texture* ambient_texture;
    Texture* diffuse_texture;
    Texture* specular_texture;
    Texture* normal_map;
    Texture* alpha_mask;

	PhongMaterial() :
        ambient(0),
		diffuse(0.5f),
		specular(0.5f),
		shininess(10),
        castShadow(true),
        ambient_texture(NULL),
        diffuse_texture(NULL),
        specular_texture(NULL),
        normal_map(NULL),
        alpha_mask(NULL)
    {}

    virtual void bindAttributes(Shader* myShader = NULL);

    virtual unsigned int getFlags();
    Shader* getShader() {return shader;}

    /**
     * deprecated, you should use setDiffuseTexture instead
     */
    void setTexture(Texture* myTexture);

    void setAmbientTexture(Texture* myTexture);
    void setDiffuseTexture(Texture* myTexture);
    void setSpecularTexture(Texture* myTexture);
    void setNormalMap(Texture* myNormalMap);
    void setAlphaMask(Texture* myAlphaMask);

private:
    Shader* shader;
};

#endif // PHONGMATERIAL_H
