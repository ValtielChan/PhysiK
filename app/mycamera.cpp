#include "mycamera.h"
#include <glm/ext.hpp>

#define SCROLL_SPEED 0.95f
#define ROTATION_SPEED 0.01f

MyCamera::MyCamera(float myFov, float myNear, float myFar) :
    m_fov(myFov), m_near(myNear), m_far(myFar)
{
    reset();
    resize(800, 600);
}

void MyCamera::resize(int width, int height)
{
    if(width > 0 && height > 0)
        m_projection = glm::perspectiveFov<float>(m_fov, float(width), float(height), m_near, m_far);
}

void MyCamera::mouseMove(float dx, float dy)
{
    m_rotation.x += dx*ROTATION_SPEED;
    m_rotation.y += dy*ROTATION_SPEED;
    if(m_rotation.y > 3.14f)
        m_rotation.y = 3.14f;
    if(m_rotation.y < -3.14f)
        m_rotation.y = -3.14f;
    computeView();
}

void MyCamera::reset()
{
    m_center = glm::vec3(-5);
    m_rotation = glm::vec2(0, 0);
    m_dist = 10;
    computeView();
}

void MyCamera::computeView()
{

    m_view = glm::translate(glm::mat4(), glm::vec3(0, 0, -m_dist));
    m_view = glm::rotate(m_view, m_rotation.y, glm::vec3(1, 0, 0));
    m_view = glm::rotate(m_view, m_rotation.x, glm::vec3(0, 1, 0));
    m_view = glm::translate(m_view, m_center);
}

void MyCamera::mouseScroll(int nbScrolls)
{
    nbScrolls /= 120;
    if(nbScrolls > 0)
        m_dist *= nbScrolls*SCROLL_SPEED;
    else
        m_dist /= -nbScrolls*SCROLL_SPEED;
    computeView();
}
