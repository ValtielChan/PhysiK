#ifndef PARAMETRIC_MESH_H
#define PARAMETRIC_MESH_H

#include "meshbuilder.h"

class Sphere : public MeshBuilder
{
private:
    class Edge{
    public:
        int b;
        int vertex;
        Edge* next;
        Edge(int myB = -1, int myVertex = -1) :
            b(myB),
            vertex(myVertex),
            next(NULL)
        {}
        ~Edge(){ if(next != NULL) delete(next); }
    };

    Edge* edges;
    float radius;

    int getEdge(int a, int b);
    void createVertex(float u, float v);
    void subdivide();
public:
    Sphere(Material* mat, int n = 0, float myRadius = 1.f);
};

class GridMesh : public MeshBuilder
{
public:
    GridMesh(Material* mat, int width, int height, bool alternate = true);
private:
    int getVertexId(int i, int j, int height);
};

#endif // PARAMETRIC_MESH_H
