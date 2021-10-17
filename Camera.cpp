#include "pch.h"
#include "Camera.h"

PAG::Camera::Camera()
{

}

PAG::Camera::Camera(float w, float h): wViewport(w), hViewport(h)
{

}

PAG::Camera::Camera(glm::vec3 p, float a, float zn, float zf, float w, float h): pos(p), fov(a), zNear(zn), zFar(zf), wViewport(w), hViewport(h)
{
}

glm::vec3 PAG::Camera::getUp()
{
	return up;
}

glm::vec3 PAG::Camera::cameraDirection()
{
	return glm::normalize(target - pos);
}

glm::mat4 PAG::Camera::getViewMatrix()
{
	return glm::lookAt(pos, target, up);
}

glm::mat4 PAG::Camera::getProjMatrix()
{
	proj = glm::perspective(glm::radians(fov), wViewport / hViewport, zNear, zFar);
	return proj;
}


PAG::Camera::~Camera()
{

}

void PAG::Camera::setViewport(float w, float h)
{
	wViewport = w;
	hViewport = h;
}
