#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <SparrowRenderer/camera.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class QElapsedTimer;

class MyCamera : public Camera
{
    glm::mat4 m_view;
    glm::mat4 m_projection;

    float m_fov;
    float m_near;
    float m_far;

    // camera position
    glm::vec3 m_center;
    glm::vec2 m_rotation;
    float m_dist;

    // camera movement
    glm::vec3 m_origin;
    glm::vec3 m_target;
    QElapsedTimer* timer;

public:
    MyCamera(float myFov = 70.f, float myNear = 0.1f, float myFar = 10000.f);

    glm::mat4 getProjectionMatrix() {return m_projection;}
    glm::mat4 getViewMatrix() {return m_view;}

    void resize(int width, int height);

    void rotateCamera(float dx, float dy);
    void moveCamera(float dx, float dy);
    void reset();
    void computeView();
    void mouseScroll(int nbScrolls);
    void setCenter(glm::vec3 pos) {m_center = pos;computeView();}
    void moveCenter(glm::vec3 pos) {m_center += pos;computeView();}
    glm::vec3 getDefaultPxInfo();
    void setTarget(glm::vec3 pos);
    void update();
};

#endif // MYCAMERA_H
