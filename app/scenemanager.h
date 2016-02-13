#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <SparrowRenderer/scene.h>
#include <PhysiK/particlesystem.h>
#include "particlemesh.h"

class Mesh;

class SceneManager
{
    ArrayScene scene;
    PhysiK::ParticleSystem physics;

    std::vector<ParticleMesh*> particles;

    Mesh* createGrid(int n = 10, float size = 10);

public:
    SceneManager();
    ~SceneManager();

    void resetScene();

    void addParticleGroup(ParticleProperties properties, const glm::vec3 *positions);
    void addNode(GeometryNode* node);

    Scene* getScene() {return &scene;}

    double update(float dt);
};

#endif // SCENEMANAGER_H
