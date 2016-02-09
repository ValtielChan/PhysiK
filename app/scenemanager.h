#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <SparrowRenderer/scene.h>

class Mesh;

struct ParticleProperties
{
    int amount;
    float radius;
    float mass;
    float r;
    float g;
    float b;
};

class SceneManager
{
    ArrayScene scene;
    Mesh* particle;

public:
    SceneManager();
    ~SceneManager();

    void resetScene();

    void addParticleGroup(std::vector<glm::vec3> particles, ParticleProperties properties);
    void addParticle(glm::vec3 position, ParticleProperties properties);
    void addNode(GeometryNode* node);

    Scene* getScene() {return &scene;}
};

#endif // SCENEMANAGER_H
