#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <SparrowRenderer/scene.h>
#include <PhysiK/particlesystem.h>

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
    PhysiK::ParticleSystem physics;

    Mesh* createGrid(int n = 10, float size = 10);

public:
    SceneManager();
    ~SceneManager();

    void resetScene();

    void addParticleGroup(std::vector<glm::vec3> particles, ParticleProperties properties);
    void addParticle(glm::vec3 position, ParticleProperties properties);
    void addNode(GeometryNode* node);

    Scene* getScene() {return &scene;}

    void update(float dt);
};

#endif // SCENEMANAGER_H
