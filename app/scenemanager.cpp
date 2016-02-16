#include "scenemanager.h"

#include <SparrowRenderer/parametricmesh.h>
#include <SparrowRenderer/phongmaterial.h>

#include <glm/ext.hpp>

#include <set>
#include <chrono>

SceneManager::SceneManager()
{
    Light* key = new Light();
    key->initDirectionnalLight(glm::vec3(2, -8, -5), glm::vec3(0.9f));
    Light* back = new Light();
    back->initDirectionnalLight(glm::vec3(1, 1, 10), glm::vec3(0.18f, 0.16f, 0.096f));
    Light* fill = new Light();
    fill->initDirectionnalLight(glm::vec3(-8, 4, -1), glm::vec3(0.1f));
    scene.addLight(key);
    scene.addLight(back);
    scene.addLight(fill);
}

SceneManager::~SceneManager()
{
    // delete light
    for(SceneIterator<Light*>* lightIt = scene.getLights(); lightIt->isValid(); lightIt->next())
        delete(lightIt->getItem());
    // delete meshes
    resetScene();
}

void SceneManager::resetScene()
{
    std::set<void*> ptrs;

    for(SceneIterator<GeometryNode*>* geometryIt = scene.getGeometry();
        geometryIt->isValid(); geometryIt->next())
    {
        GeometryNode *node = geometryIt->getItem();
        std::pair<std::set<void*>::iterator, bool> ret = ptrs.emplace(node->mesh->material);
        if(ret.second)
            delete node->mesh->material;
        ret = ptrs.emplace(node->mesh->material);
        if(ret.second)
            delete node->mesh;
        delete node;
    }
    scene.clearEntities();
    GeometryNode* node = new GeometryNode();
    node->mesh = createGrid(20, 10);
    scene.addMesh(node);

    particles.clear();
    bodies.clear();

    physics.reset();
}

void SceneManager::addBody(Mesh* mesh, BodyProperties properties)
{
    mesh->initGL(!properties.isRigid);
    BodyMesh *body = new BodyMesh(mesh, properties);
    scene.addMesh(body);
    bodies.push_back(body);
    if(properties.isRigid)
        physics.addRigidBody(body->getBody());
    else
        physics.addSoftBody(body->getBody());
}

void SceneManager::addParticleGroup(ParticleProperties properties, const glm::vec3 *positions)
{
    GeometryNode* node = new GeometryNode();
    ParticleMesh* particleMesh = new ParticleMesh(properties, positions);
    particleMesh->initGL();
    node->mesh = particleMesh;
    scene.addMesh(node);
    particles.push_back(particleMesh);
    physics.addParticleGroup(particleMesh->getParticleGroup());
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
    mat->diffuse = glm::vec3(0.18f, 0.235f, 0.36f);
    mat->ambient = mat->diffuse/5;
    mat->specular = glm::vec3(0.8f);
    mat->shininess = 40;
    grid->material = mat;
    grid->initGL();
    return grid;
}

double SceneManager::update(float dt)
{
    std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
    physics.nextSimulationStep(dt);
    double timeElapsed = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - t).count();
    for(ParticleMesh *pm : particles)
        pm->updatePositions();
    for(BodyMesh *bm : bodies)
        bm->update();
    return timeElapsed;
}
