#include "mycamera.h"
#include <glm/ext.hpp>

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
    glm::rotate(m_view, dx/100.f, glm::vec3(0, 0, 1));
    glm::rotate(m_view, dy/100.f, glm::vec3(1, 0, 0));
}

void MyCamera::reset()
{
     m_view = glm::lookAt(glm::vec3(5, 5, -5), glm::vec3(5, 5, 10), glm::vec3(0, 1, 0));
}
