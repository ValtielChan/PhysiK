#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>

class Camera{
	public:
		virtual glm::mat4 getProjectionMatrix() = 0;
		virtual glm::mat4 getViewMatrix() = 0;

		virtual void resize(int width, int height) = 0;
		virtual ~Camera(){}//polymorphism
};

#endif // CAMERA_H
