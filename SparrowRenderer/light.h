#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include "camera.h"

class FrameBuffer;
class Shader;
class Scene;
class Texture;

class Light : public Camera
{
public:
    enum LightType {
        DIRECTIONNAL,
        POINT,
        SPOT
    };

    enum LightFlags {
        AMBIENT_FLAG,
        DIRECTIONNAL_FLAG,
        POINT_FLAG,
        SPOT_FLAG,
        SHADOWMAP_FLAG,
        NB_LIGHT_FLAGS
    };
    static const char* flagStr[];
    static const glm::mat4 biasMatrix;

    Light();
    void initDirectionnalLight(glm::vec3 dir = glm::vec3(0, -1, 0), glm::vec3 lightColor = glm::vec3(1));
    void initPointLight(glm::vec3 pos = glm::vec3(0), glm::vec3 lightColor = glm::vec3(1), float att = 1);
    void initSpotLight(glm::vec3 pos = glm::vec3(0), glm::vec3 dir = glm::vec3(1, 0, 0), float spotAngle = 360, glm::vec3 lightColor = glm::vec3(1));

    bool isDirectionnal();
    LightType getType() {return type;}
    glm::vec3 getDir() {return direction;}
    glm::vec3 getPos() {return position;}
    glm::vec3 getColor() {return color;}
    float getAttenuation() {return attenuation;}

    bool isShadowCaster() {return shadowCaster;}
    void initShadowMap(int resWidth, int resHeight, glm::vec3 dim = glm::vec3(1));
    void generateShadowMap(Scene* scene);
    Texture* getShadowMap();

    void setAttenuation(float a) {attenuation = a;}
    void setPosition(glm::vec3 new_pos);
    void setColor(glm::vec3 new_color) {color = new_color;}

    // camera inheritance
    glm::mat4 getProjectionMatrix() {return projectionMatrix;}
    glm::mat4 getViewMatrix() {return viewMatrix;}
    // does nothing, just required for inheriting Camera
    void    resize(int width, int height) {}

    static unsigned int getFlags(Light* l);
private:
    // standard attributes
    LightType type;

    glm::vec3 position;
    glm::vec3 direction;
    float cutOffAngle;
    float attenuation;
    glm::vec3 color;

    bool isDir;

    // shadowmap attributes
    bool shadowCaster;
    int shadowMapWidth;
    int shadowMapHeight;
    FrameBuffer* shadowMap;
    Shader* shaders[2];
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    static const char* vertSource;
    static const char* fragSource;
};

#endif // LIGHT_H
