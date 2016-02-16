#ifndef BODYMESH_H
#define BODYMESH_H

#include <SparrowRenderer/scene.h>
#include <PhysiK/body.h>

struct BodyProperties
{
    bool isRigid;
    bool isKinematic;
    float mass; // 0 if kinematic
};

class BodyMesh : public GeometryNode
{
    BodyProperties properties;
    PhysiK::Body body;

    void updateTransform();
    void updatePositions();

public:
    BodyMesh(Mesh* myMesh, BodyProperties myProperties);

    void update();

    PhysiK::Body* getBody();
};

#endif // BODYMESH_H
