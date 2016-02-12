#include "mesh.h"
#include <glm/ext.hpp>
#include <set>
#include "glassert.h"
#include "sparrowrenderer.h"
#include "material.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Mesh::Mesh() :
    material(NULL),
    vao(0),
    nb_buffers(0),
    primitive_type(GL_TRIANGLES)
{}

Mesh::~Mesh()
{
    destroyGL();
}

void Mesh::initGL(bool isDynamic)
{
    if(vao != 0)
        destroyGL();

    GLenum buffer_type = isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // create VAO
    glAssert(glGenVertexArrays(1, &vao));
    glAssert(glBindVertexArray(vao));

    // create VBOs
    glAssert(glGenBuffers(NB_BUFFERS, vbo));

    // init indices vbos
    glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDICES_BUFFER]));
    glAssert(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), buffer_type));

    // init positions vbo
    glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION_BUFFER]));
    glAssert(glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), buffer_type));

    if(hasNormals())
    {
        // init normals vbo
        glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]));
        glAssert(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), buffer_type));
    }

    if(hasTexCoords())
    {
        // init texCoords vbo
        glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]));
        glAssert(glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), buffer_type));
    }

    if(hasTangents())
    {
        // init tangents vbo
        glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[TANGENT_BUFFER]));
        glAssert(glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3)*2, tangents.data(), buffer_type));
    }

    if(hasInstances() && SparrowRenderer::isModernOpenGLAvailable())
    {
        // init instances vbo
        glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[INSTANCE_BUFFER]));
        glAssert(glBufferData(GL_ARRAY_BUFFER, instances_offsets.size() * sizeof(glm::vec3), instances_offsets.data(), GL_DYNAMIC_DRAW));
    }

    // unbind vao
    glAssert(glBindVertexArray(0));
    glAssert(glBindBuffer(GL_ARRAY_BUFFER, 0));
    glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Mesh::draw(Shader* shader, bool drawNormals, bool drawTexCoord, bool drawTangents)
{
    bool crappy = (shader == NULL);
    material->bindAttributes(shader);
    glAssert(glBindVertexArray(vao));
    glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION_BUFFER]));
    if(crappy)
    {
        glAssert(glEnableClientState(GL_VERTEX_ARRAY));
        glAssert(glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0)));
    }
    else
    {
        glAssert(glEnableVertexAttribArray(0));
		glAssert(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), BUFFER_OFFSET(0)));
    }
    if(hasNormals() && drawNormals)
    {
        glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]));
        if(crappy)
        {
            glAssert(glEnableClientState(GL_NORMAL_ARRAY));
            glAssert(glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0)));
        }
        else
        {
            glAssert(glEnableVertexAttribArray(1));
            glAssert(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), BUFFER_OFFSET(0)));
        }
    }
    if(hasTexCoords() && drawTexCoord)
    {
        glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]));
        if(crappy)
        {
            glAssert(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
            glAssert(glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0)));
        }
        else
        {
            glAssert(glEnableVertexAttribArray(2));
            glAssert(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), BUFFER_OFFSET(0)));
        }
    }
    if(hasTangents() && drawTangents && !crappy)
    {
        glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[TANGENT_BUFFER]));
        glAssert(glEnableVertexAttribArray(3));
        glAssert(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Tangents), BUFFER_OFFSET(0)));
        glAssert(glEnableVertexAttribArray(4));
        glAssert(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Tangents), BUFFER_OFFSET(sizeof(glm::vec3))));
    }
    if(!instances_offsets.empty() && !crappy)
    {
        glAssert(glBindBuffer(GL_ARRAY_BUFFER, vbo[INSTANCE_BUFFER]));
        glAssert(glEnableVertexAttribArray(5));
        glAssert(glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), BUFFER_OFFSET(0)));
        glAssert(glVertexAttribDivisor(5, 1));
        glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDICES_BUFFER]));
        glAssert(glDrawElementsInstanced(primitive_type, indices.size(), GL_UNSIGNED_INT, NULL, instances_offsets.size()));
	}
	else
	{
		glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDICES_BUFFER]));
		glAssert(glDrawElements(primitive_type, indices.size(), GL_UNSIGNED_INT, NULL));
	}
	glAssert(glBindVertexArray(0));
	if(crappy)
    {
        glAssert(glDisableClientState(GL_VERTEX_ARRAY));
        if(hasNormals() && drawNormals)
            glAssert(glDisableClientState(GL_NORMAL_ARRAY));
        if(hasTexCoords() && drawTexCoord)
            glAssert(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
    }
}

void Mesh::destroyGL()
{
    if(vbo != NULL)
    {
        glAssert(glDeleteVertexArrays(1, &vao));
        glAssert(glDeleteBuffers(NB_BUFFERS, vbo));
        vao = 0;
    }
}

struct VertexComparator
{
    // c'est plutot crade mais j'ai pas trouve d'autre moyen pour le moment
    static Mesh* mesh;
    static void setMesh(Mesh* m) {VertexComparator::mesh = m;}

    bool operator() (const int& vertId1, const int& vertId2) const
    {
        if(mesh->positions[vertId1].x != mesh->positions[vertId2].x)
            return (mesh->positions[vertId1].x < mesh->positions[vertId2].x);
        if(mesh->positions[vertId1].y != mesh->positions[vertId2].y)
            return (mesh->positions[vertId1].y < mesh->positions[vertId2].y);
        if(mesh->positions[vertId1].z != mesh->positions[vertId2].z)
            return (mesh->positions[vertId1].z < mesh->positions[vertId2].z);
        if(mesh->hasTexCoords())
        {
            if(mesh->texCoords[vertId1].x != mesh->texCoords[vertId2].x)
                return (mesh->texCoords[vertId1].x < mesh->texCoords[vertId2].x);
            if(mesh->texCoords[vertId1].y != mesh->texCoords[vertId2].y)
                return (mesh->texCoords[vertId1].y < mesh->texCoords[vertId2].y);
        }
        if(mesh->hasNormals())
        {
            if(mesh->normals[vertId1].x != mesh->normals[vertId2].x)
                return (mesh->normals[vertId1].x < mesh->normals[vertId2].x);
            if(mesh->normals[vertId1].y != mesh->normals[vertId2].y)
                return (mesh->normals[vertId1].y < mesh->normals[vertId2].y);
            if(mesh->normals[vertId1].z != mesh->normals[vertId2].z)
                return (mesh->normals[vertId1].z < mesh->normals[vertId2].z);
        }
        return false;
    }
};

Mesh* VertexComparator::mesh = NULL;

void Mesh::mergeVertices()
{
    bool *deleted = new bool[positions.size()];
    int *offsets = new int[positions.size()];
    std::set<int, VertexComparator> vertexSet;
    VertexComparator::setMesh(this);

    for(std::size_t i=0; i<indices.size(); ++i)
    {
        std::pair<std::set<int,VertexComparator>::iterator,bool> ret = vertexSet.insert(indices[i]);
        deleted[indices[i]] = !ret.second && *(ret.first) != indices[i];
        if(deleted[indices[i]])
        {
            if(hasTangents())
            {
                tangents[*(ret.first)].tangent += tangents[indices[i]].tangent;
                tangents[*(ret.first)].binormal += tangents[indices[i]].binormal;
            }
            indices[i] = *(ret.first);
        }
    }
    int offset = 0;
    int pos = 0;
    for(std::size_t i=0; i<positions.size(); ++i)
    {
        if(deleted[i])
            ++offset;
        else
        {
            offsets[i] = offset;
            if(offset != 0)
            {
                positions[pos] = positions[i];
                if(hasTexCoords())
                    texCoords[pos] = texCoords[i];
                if(hasNormals())
                    normals[pos] = normals[i];
                if(hasTangents())
                    tangents[pos] = tangents[i];
            }
            ++pos;
        }
    }
    for(std::size_t i=0; i<indices.size(); ++i)
        indices[i] -= offsets[indices[i]];

    positions.resize(positions.size()-offset);
    if(hasTexCoords())
        texCoords.resize(texCoords.size()-offset);
    if(hasNormals())
        normals.resize(normals.size()-offset);
    if(hasTangents())
        tangents.resize(tangents.size()-offset);
    for(Tangents &t : tangents)
    {
        t.tangent = glm::normalize(t.tangent);
        t.binormal = glm::normalize(t.binormal);
    }
}

void Mesh::computeNormals()
{
    normals.resize(positions.size());
    for (std::size_t i=0; i < indices.size(); i += 3)
    {
        int v0 = indices[i];
        int v1 = indices[i+1];
        int v2 = indices[i+2];
        glm::vec3 n = glm::cross(positions[v1] - positions[v0], positions[v2] - positions[v0]);
        normals[v0] += n;
        normals[v1] += n;
        normals[v2] += n;
    }
    for(glm::vec3 &n : normals)
        n = glm::normalize(n);
}

void Mesh::computeTangents()
{
    if(!hasTexCoords())
        return;
    tangents = std::vector<Tangents>(positions.size());

    for (std::size_t j=0; j < indices.size(); j += 3)
    {
        int vertexId0 = indices[j];
        int vertexId1 = indices[j+1];
        int vertexId2 = indices[j+2];

        const glm::vec3 &v1 = positions[vertexId0];
        const glm::vec3 &v2 = positions[vertexId1];
        const glm::vec3 &v3 = positions[vertexId2];

        const glm::vec2& w1 = texCoords[vertexId0];
        const glm::vec2& w2 = texCoords[vertexId1];
        const glm::vec2& w3 = texCoords[vertexId2];

        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;

        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;

        float r = 1.0f / (s1 * t2 - s2 * t1);
        glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);

        Tangents& tan1 = tangents[vertexId0];
        Tangents& tan2 = tangents[vertexId1];
        Tangents& tan3 = tangents[vertexId2];

        tan1.tangent += sdir;
        tan2.tangent += sdir;
        tan3.tangent += sdir;

        tan1.binormal += tdir;
        tan2.binormal += tdir;
        tan3.binormal += tdir;
    }
}
