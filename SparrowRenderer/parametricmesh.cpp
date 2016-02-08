#include "parametricmesh.h"
#include "glm/ext.hpp"
#include "phongmaterial.h"

#define M_PI 3.14159265358979323846
#define MAGIC_RATIO 0.37139f

Sphere::Sphere(Material* mat, int n)
{
    // icosahedron :

    // top cap
    createVertex(0, 1);
    for(int i=0; i<5; i++)
        createVertex(i/5.f, 1-MAGIC_RATIO);

    // bottom cap
    createVertex(0, 0);
    for(int i=0; i<5; i++)
        createVertex((i+0.5f)/5.f, MAGIC_RATIO);

    // Compute faces
    for(int i=0; i<5; i++){
        int top = 1;
        int bottom = 7;
        int offset = (i+1)%5;
        // top cap
		addTriangle(0, top+i, top+offset);
        // bottom cap
		addTriangle(6, bottom+offset, bottom+i);
        // middle ribbon
		addTriangle(top+i, bottom+i, top+offset);
		addTriangle(top+offset, bottom+i, bottom+offset);
    }

    // geodesic subdivisions :
    for(int i=0; i<n; i++)
        subdivide();
}

int Sphere::getEdge(int a, int b)
{
    Edge* current = edges+a;
    int vid = -1;
    while(vid == -1)
    {
        if(current->b == b)
            vid = current->vertex;
        else if(current->next == NULL)
        {
			vid = positions.size();
			// creating subdivision vertex
			glm::vec3 pos = glm::normalize(positions[a] + positions[b] / 2.f);
			addPosition(pos);
			addNormal(pos);

            // u/v sphériques, cohérents sur toute la sphère sauf des artefacts au niveau des u==0
			float newU = (pos.x < 0 ? 1.5f : 1.f) + atan(pos.z/pos.x)/(2*M_PI);
			float newV = acos(pos.y)/M_PI;
			addTexCoord(newU - floor(newU), newV);
            // alternative, u/v moyennés :
            //v.texCoord = (v0.texCoord + v1.texCoord)/2.f;

            // inserting the new vertex in the edge collection
            if(current->b == -1)
            {
                current->vertex = vid;
                current->b = b;
            }
            else
                current->next = new Edge(b, vid);
        }
        else
            current = current->next;
    }
    return vid;
}

void Sphere::subdivide()
{
    edges = new Edge[positions.size()-1];
    int nb_triangles = indices.size()/3;
    for(int j=0; j<nb_triangles; j++)
    {
        int vid[3];
        for(int k=0; k<3; k++)
        {
            int idA = indices[j*3 + k];
            int idB = indices[j*3 + (k+1)%3];
            int a = idA < idB ? idA : idB;
            int b = idA > idB ? idA : idB;
            vid[k] = getEdge(a, b);
        }
        for(int k=0; k<3; k++)
            addTriangle(indices[j*3 + k], vid[k], vid[(k+2)%3]);
        addTriangle(vid[0], vid[1], vid[2]);
    }
    delete[](edges);
    indices.erase(indices.begin(), indices.begin()+nb_triangles*3);
}

void Sphere::createVertex(float u, float v)
{
    glm::vec3 pos(cos(u*2*M_PI)*sin(v*M_PI),
                  cos(v*M_PI),
                  sin(u*2*M_PI)*sin(v*M_PI));
    addPosition(pos);
    addNormal(pos);
    addTexCoord(u, v);
}

GridMesh::GridMesh(Material* mat, int width, int height, bool alternate)
{
    setMaterial(mat);
    for(int i=0; i<=width; ++i)
    {
        for(int j=0; j<=height; ++j)
        {
            float x = (float)i/(float)width;
            float y = (float)j/(float)height;
            addPosition(x-0.5f, 0, y-0.5f);
            addNormal(0, 1, 0);
            addTexCoord(x, y);
            if(i > 0 && j > 0)
            {
                if(alternate && (i+j)%2)
                {
                    addTriangle(getVertexId(i, j, height),
                                getVertexId(i, j-1, height),
                                getVertexId(i-1, j, height));
                    addTriangle(getVertexId(i-1, j-1, height),
                                getVertexId(i-1, j, height),
                                getVertexId(i, j-1, height));
                }
                else
                {
                    addTriangle(getVertexId(i, j, height),
                                getVertexId(i-1, j-1, height),
                                getVertexId(i-1, j, height));
                    addTriangle(getVertexId(i, j, height),
                                getVertexId(i, j-1, height),
                                getVertexId(i-1, j-1, height));
                }
            }
        }
    }
}

int GridMesh::getVertexId(int i, int j, int height)
{
    return i*(height+1) + j;
}
