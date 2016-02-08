#include "scenemanager.h"

#include <SparrowRenderer/parametricmesh.h>
#include <SparrowRenderer/phongmaterial.h>

SceneManager::SceneManager()
{
    Light* sun = new Light();
    sun->initDirectionnalLight(glm::vec3(3, -10, -2));
    scene.addLight(sun);
}

SceneManager::~SceneManager()
{
    // delete light
    delete(scene.getLights()->getItem());
    // delete meshes
    resetScene();
}

void SceneManager::resetScene()
{
    for(SceneIterator<GeometryNode*>* geometryIt = scene.getGeometry();
        geometryIt->isValid(); geometryIt->next())
    {
        GeometryNode *node = geometryIt->getItem();
        delete node->mesh->material;
        delete node->mesh;
        delete node;
    }
    scene.clearEntities();
}

void SceneManager::addParticleGroup(std::vector<glm::vec3> particles)
{
    if(particles.size() > 1)
    {
        PhongMaterial *mat = new PhongMaterial();
        mat->ambient = glm::vec3(0.1f);
        mat->diffuse = glm::vec3(0.f, 0.f, 0.8f);
        mat->specular = glm::vec3(1.f);
        mat->shininess = 20;

        GeometryNode* node = new GeometryNode();
        Sphere* sphere = new Sphere(mat, 1);
        sphere->texCoords.clear();
        sphere->instances_offsets = particles;
        sphere->initGL();
        node->mesh = sphere;

        node->modelMatrix = glm::mat4(); // must not move
        scene.addMesh(node);
    }
}
