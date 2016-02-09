#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <SparrowRenderer/camera.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class MyCamera : public Camera
{
    glm::mat4 m_view;
    glm::mat4 m_projection;

    float m_fov;
    float m_near;
    float m_far;

    glm::vec3 m_center;
    glm::vec2 m_rotation;
    float m_dist;

public:
    MyCamera(float myFov = 90.f, float myNear = 0.1f, float myFar = 10000.f);

    virtual glm::mat4 getProjectionMatrix() {return m_projection;}
    virtual glm::mat4 getViewMatrix() {return m_view;}

    virtual void resize(int width, int height);

    void mouseMove(float dx, float dy);
    void reset();
    void computeView();
    void mouseScroll(int nbScrolls);
};

#endif // MYCAMERA_H
