#ifndef SCENE_H
#define SCENE_H

#include <glm/mat4x4.hpp>
#include "camera.h"
#include <vector>
#include "light.h"

class Mesh;

// Scene interface :

template <class T>
class SceneIterator
{
public:
    virtual SceneIterator& operator++() = 0;
    virtual T operator*() = 0;
    virtual bool isValid() = 0;
    void next() {if(isValid()) operator++();}
    T getItem() {return operator*();}
};

struct GeometryNode
{
    Mesh* mesh;
    glm::mat4 modelMatrix;
};

class Scene
{
public:
    virtual SceneIterator<Light*>* getLights() = 0;
    virtual SceneIterator<GeometryNode*>* getGeometry() = 0;
};

// Some basic implementations :

template <class T>
class ArraySceneIterator : public SceneIterator<T>
{
    std::vector<T> &vec;
    int id;
public:
    ArraySceneIterator(std::vector<T> &myVec, int myId=0) : vec(myVec), id(myId) {}
    virtual SceneIterator<T>& operator++() {++id; return *this;}
    virtual T operator*() {return vec[id];}
    virtual bool isValid() {return id < vec.size();}
};

class ArrayScene : public Scene
{
protected:
    std::vector<Light*> lights;
    std::vector<GeometryNode*> geometry;
public:
    void clearLights() {lights.clear();}
    void clearEntities() {geometry.clear();}
    void clearScene() {clearLights(); clearEntities();}
    void addMesh(GeometryNode* node) {geometry.push_back(node);}
    void addLight(Light* myLight) {lights.push_back(myLight);}
    Mesh* getMesh(int id) {return geometry[id]->mesh;}

    virtual SceneIterator<Light*>* getLights() {return new ArraySceneIterator<Light*>(lights);}
    virtual SceneIterator<GeometryNode*>* getGeometry() {return new ArraySceneIterator<GeometryNode*>(geometry);}
};

#endif // SCENE_H
