#include "mycamera.h"
#include <glm/ext.hpp>

#include <QElapsedTimer>

#define SCROLL_SPEED 0.998f
#define ROTATION_SPEED 0.01f
#define MOVE_SPEED 0.002f

MyCamera::MyCamera(float myFov, float myNear, float myFar) :
    m_fov(myFov), m_near(myNear), m_far(myFar), timer(NULL)
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
    if(m_rotation.y > 1.57f)
        m_rotation.y = 1.57f;
    if(m_rotation.y < -1.57f)
        m_rotation.y = -1.57f;
    computeView();
}

void MyCamera::reset()
{
    m_center = glm::vec3(0, 4, 0);
    m_target = m_center;
    m_rotation = glm::vec2(0, 1);
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

glm::vec3 MyCamera::getDefaultPxInfo()
{
    glm::vec4 plop(m_center, 1);
    plop = (m_projection * m_view) * plop;
    return glm::vec3(m_dist/m_far, 1/plop.w, 0);
}

void MyCamera::setTarget(glm::vec3 pos)
{
    m_origin = m_center;
    m_target = pos;
    if(timer != NULL)
        delete timer;
    timer = new QElapsedTimer();
    timer->start();
}

void MyCamera::update()
{
    if(timer != NULL)
    {
        double secondsElapsed = timer->nsecsElapsed()*0.000000001;
        if(secondsElapsed >= 1)
        {
            setCenter(m_target);
            delete timer;
            timer = NULL;
        }
        else
        {
            float ratio = (cos(secondsElapsed*3.1416)+1)/2;
            setCenter(m_origin*ratio + m_target*(1-ratio));
        }
    }
}
