#include "scenemanager.h"

#include <SparrowRenderer/parametricmesh.h>
#include <SparrowRenderer/phongmaterial.h>

#include <glm/ext.hpp>

SceneManager::SceneManager()
{
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
    GeometryNode* node = new GeometryNode();
    node->mesh = createGrid(20, 10);
    scene.addMesh(node);
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

Mesh* SceneManager::createGrid(int n, float size)
{
    Mesh* grid = new Mesh();
    float step = 2.f*size/float(n);
    for(int i=0; i<=n; ++i)
    {
        grid->positions.push_back(glm::vec3( size, 0, -size+i*step));
        grid->positions.push_back(glm::vec3(-size, 0, -size+i*step));
        grid->normals.push_back(glm::vec3(0, 1, 0));
        grid->normals.push_back(glm::vec3(0, 1, 0));
        grid->indices.push_back(i*4);
        grid->indices.push_back(i*4 + 1);
        grid->positions.push_back(glm::vec3(-size+i*step, 0,  size));
        grid->positions.push_back(glm::vec3(-size+i*step, 0, -size));
        grid->normals.push_back(glm::vec3(0, 1, 0));
        grid->normals.push_back(glm::vec3(0, 1, 0));
        grid->indices.push_back(i*4 + 2);
        grid->indices.push_back(i*4 + 3);
    }
    grid->setPrimitiveType(GL_LINES);
    PhongMaterial *mat = new PhongMaterial();
    mat->ambient = glm::vec3(0.1f);
    mat->diffuse = glm::vec3(0.5f);
    mat->specular = glm::vec3(0.5f);
    mat->shininess = 10;
    grid->material = mat;
    grid->initGL();
    return grid;
}
