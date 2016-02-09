#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <SparrowRenderer/scene.h>

class Mesh;

class SceneManager
{
    ArrayScene scene;
    Mesh* particle;

public:
    SceneManager();
    ~SceneManager();

    void resetScene();

    void addParticleGroup(std::vector<glm::vec3> particles);
    void addNode(GeometryNode* node);

    Scene* getScene() {return &scene;}
};

#endif // SCENEMANAGER_H
