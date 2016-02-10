#include "scenemanager.h"

#include <SparrowRenderer/parametricmesh.h>
#include <SparrowRenderer/phongmaterial.h>

#include <glm/ext.hpp>

SceneManager::SceneManager()
{
    particle = NULL;
    Light* sun = new Light();
    sun->initDirectionnalLight(glm::vec3(2, -5, -8));
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

void SceneManager::addParticleGroup(std::vector<glm::vec3> particles, ParticleProperties properties)
{
    if(particles.size() > 1)
    {
        PhongMaterial *mat = new PhongMaterial();
        mat->ambient = glm::vec3(0.1f);
        mat->diffuse = glm::vec3(properties.r, properties.g, properties.b);
        mat->specular = glm::vec3(1.f);
        mat->shininess = 20;

        GeometryNode* node = new GeometryNode();
        Sphere* sphere = new Sphere(mat, 1, properties.radius);
        sphere->texCoords.clear();
        sphere->instances_offsets = particles;
        sphere->initGL();
        node->mesh = sphere;

        scene.addMesh(node);
    }
    else
        addParticle(particles[0], properties);
}

void SceneManager::addParticle(glm::vec3 position, ParticleProperties properties)
{
    PhongMaterial *mat = new PhongMaterial();
    mat->ambient = glm::vec3(0.1f);
    mat->diffuse = glm::vec3(properties.r, properties.g, properties.b);
    mat->specular = glm::vec3(1.f);
    mat->shininess = 10;

    Sphere* sphere = new Sphere(mat, 1, properties.radius);
    sphere->texCoords.clear();
    sphere->initGL();

    GeometryNode* node = new GeometryNode();
    node->mesh = sphere;
    node->modelMatrix = glm::translate(glm::mat4(), position);
    scene.addMesh(node);
}

void SceneManager::addNode(GeometryNode* node)
{
    scene.addMesh(node);
}
