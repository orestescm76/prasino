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

void PAG::Camera::pan(float degrees, Direction dir)
{
	//1 transladar el punto donde miro a donde esta la camara. o transladar la camara en el origen
	//2 rotacion
	//3 transladar todo a donde estaba
	glm::vec3 n = -cameraDirection(true); //direccion camara
	glm::vec3 u = glm::normalize(glm::cross(up, n));
	glm::vec3 v = glm::cross(u,n);
	glm::mat4 trans1 = glm::translate(glm::mat4(1.0f), -pos);
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), v);
	glm::mat4 trans2 = glm::translate(glm::mat4(1.0f), pos);
	//Composición de la matriz
	target = glm::vec3(trans2 *rot *trans1 *glm::vec4(target,1.0f));
	std::cout << target.x << "," << target.y << "," << target.z << std::endl;
	std::cout << "Grados: " << glm::radians(degrees) << std::endl;
}

float PAG::Camera::getPanAngle()
{
	return panAngle;
}

glm::vec3 PAG::Camera::cameraDirection(bool norm)
{
	if (norm)
		return glm::normalize(pos - target);
	else
		return pos - target;
}

glm::mat4 PAG::Camera::getViewMatrix()
{
	return glm::lookAt(pos, target, up);
}

glm::mat4 PAG::Camera::getProjMatrix()
{
	return glm::perspective(glm::radians(fov), wViewport / hViewport, zNear, zFar);
}

void PAG::Camera::setViewport(float w, float h)
{
	wViewport = w;
	hViewport = h;
}

PAG::Camera::~Camera()
{

}

