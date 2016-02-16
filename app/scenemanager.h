#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <SparrowRenderer/scene.h>
#include <PhysiK/particlesystem.h>

#include "particlemesh.h"
#include "bodymesh.h"

class Mesh;
class BodyMesh;

class SceneManager
{
    ArrayScene scene;
    PhysiK::ParticleSystem physics;

    std::vector<ParticleMesh*> particles;
    std::vector<BodyMesh*> bodies;

    Mesh* createGrid(int n = 10, float size = 10);

public:
    SceneManager();
    ~SceneManager();

    void resetScene();

    void addBody(Mesh* mesh, BodyProperties properties);
    void addParticleGroup(ParticleProperties properties, const glm::vec3 *positions);
    void addNode(GeometryNode* node);

    Scene* getScene() {return &scene;}

    double update(float dt);

    void setNbIterations(int nb) {physics.setNbIterations(nb);}
};

#endif // SCENEMANAGER_H
