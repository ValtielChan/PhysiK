#include "mycamera.h"
#include <glm/ext.hpp>

#define SCROLL_SPEED 0.998f
#define ROTATION_SPEED 0.01f
#define MOVE_SPEED 0.002f

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

void MyCamera::rotateCamera(float dx, float dy)
{
    m_rotation.x += dx*ROTATION_SPEED;
    m_rotation.y += dy*ROTATION_SPEED;
    if(m_rotation.y > 3.14f)
        m_rotation.y = 3.14f;
    if(m_rotation.y < -3.14f)
        m_rotation.y = -3.14f;
    computeView();
}

void MyCamera::moveCamera(float dx, float dy)
{
    glm::vec3 moveVector(dx, 0, dy);
    moveVector = glm::inverse(glm::mat3(m_view)) * moveVector;
    m_center.x -= moveVector.x*m_dist*MOVE_SPEED;
    m_center.z -= moveVector.z*m_dist*MOVE_SPEED;
    computeView();
}

void MyCamera::reset()
{
    m_center = glm::vec3(0, 0, 0);
    m_rotation = glm::vec2(0, 0);
    m_dist = 20;
    computeView();
}

void MyCamera::computeView()
{

	m_view = glm::translate(glm::mat4(), glm::vec3(0, 0, -m_dist));
    m_view = glm::rotate(m_view, m_rotation.y, glm::vec3(1, 0, 0));
    m_view = glm::rotate(m_view, m_rotation.x, glm::vec3(0, 1, 0));
    m_view = glm::translate(m_view, -m_center);
}

void MyCamera::mouseScroll(int nbScrolls)
{
	float dstScroll;

	while(nbScrolls != 0)
	{
		if(nbScrolls > 0)
		{
			m_dist *= SCROLL_SPEED;
			--nbScrolls;
		}
		else
		{
			m_dist /= SCROLL_SPEED;
			++nbScrolls;
		}
	}

    computeView();
}
