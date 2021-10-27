#include "pch.h"
#include "Camera.h"
PAG::Camera::Camera()
{

}

PAG::Camera::Camera(float w, float h): wViewport(w), hViewport(h)
{

}

PAG::Camera::Camera(glm::vec3 p, float a, float zn, float zf, float w, float h): pos(p), fovX(a), zNear(zn), zFar(zf), wViewport(w), hViewport(h)
{
}

glm::vec3 PAG::Camera::getUp()
{
	return up;
}

void PAG::Camera::setMov(MovType mov)
{
	movType = mov;
}

void PAG::Camera::move(float xoffset, float yoffset)
{
	switch (movType)
	{
	case MovType::PAN:
		pan(xoffset * sensitivity);
		break;
	case MovType::TILT:
		tilt(yoffset*sensitivity);
		break;
	case MovType::ORBIT:
		/*orbit(xoffset, yoffset);*/
		pan(xoffset * sensitivity);
		tilt(yoffset * sensitivity);
		break;
	default:
		break;
	}
}

void PAG::Camera::move(int action)
{
	switch (movType)
	{
	case PAG::MovType::DOLLY:
		dolly(action);
		break;
	case PAG::MovType::CRANE:
		crane(action);
		break;
	default:
		break;
	}
}

void PAG::Camera::updateCameraAxis()
{
	n = -cameraDirection(true); //direccion camara
	glm::bvec3 check = glm::equal(n, up, 0.01f); //What's going on, Truth? Who was them dudes? 
	glm::bvec3 check2 = glm::equal(n, -up, 0.01f);	
	if (check.x && check.y && check.z)	   //Don't go there man
		return;							   //Now what if I told you we never went to the moon, JFK lives in Scotland with Janis Joplin, 
	if (check2.x && check2.y && check2.z)  //and the only reason we have been in the cold war for the last 45 years was because snake-headed aliens ran the oil business.
		return;							   //I think you popped another microdot.
	u = glm::normalize(glm::cross(up, n));
	v = glm::cross(u, n);
}

float PAG::Camera::getFovY()
{
	float aspect = wViewport / hViewport;
	return 2 * glm::atan(glm::tan(glm::radians(fovX) / 2) / aspect);
}

void PAG::Camera::pan(float offset)
{
	//1 transladar el punto donde miro a donde esta la camara. o transladar la camara en el origen
	//2 rotacion
	//3 transladar todo a donde estaba
	updateCameraAxis();
	//paso 1
	glm::mat4 trans1 = glm::translate(glm::mat4(1.0f), -pos);
	//paso 2
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(offset), v);
	//paso 3
	glm::mat4 trans2 = glm::translate(glm::mat4(1.0f), pos);
	//Composición de la matriz
	target = glm::vec3(trans2 *rot *trans1 *glm::vec4(target,1.0f));
	//std::cout << "(" << target.x << "," << target.y << "," << target.z << ")" << std::endl;
}

void PAG::Camera::tilt(float offset)
{
	//1 transladar el punto donde miro a donde esta la camara. o transladar la camara en el origen
	//2 rotacion
	//3 transladar todo a donde estaba
	float degrees = offset * 0.01f;
	updateCameraAxis();
	//paso 1
	glm::mat4 trans1 = glm::translate(glm::mat4(1.0f), -pos);
	//paso 2
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(offset), u);
	//paso 3
	glm::mat4 trans2 = glm::translate(glm::mat4(1.0f), pos);
	//Composición de la matriz
	target = glm::vec3(trans2 * rot * trans1 * glm::vec4(target, 1.0f));
	//std::cout << "(" << target.x << "," << target.y << "," << target.z << ")" << std::endl;
}

void PAG::Camera::crane(int direction)
{
	if (direction == GLFW_KEY_UP)
	{
		target.y += 0.5f;
		pos.y += 0.5f;
	}
	else if (direction == GLFW_KEY_DOWN)
	{
		target.y -= 0.5f;
		pos.y -= 0.5f;
	}
}

void PAG::Camera::dolly(int direction)
{
	switch (direction)
	{
	case GLFW_KEY_LEFT:
		target.x -= 0.5f;
		pos.x -= 0.5f;
		break;
	case GLFW_KEY_RIGHT:
		target.x += 0.5f;
		pos.x += 0.5f;
		break;
	case GLFW_KEY_W:
		target.z += 0.5f;
		pos.z += 0.5f;
		break;
	case GLFW_KEY_S:
		target.z -= 0.5f;
		pos.z -= 0.5f;
		break;
	}
}

void PAG::Camera::orbit(float xoffset, float yoffset)
{
	//1 transladar el punto donde miro a donde esta la camara. o transladar la camara en el origen
	//2 rotacion
	//3 transladar todo a donde estaba
	float xdegrees = xoffset * 0.01f;
	float ydegrees = yoffset * 0.01f;
	updateCameraAxis();
	//paso 1
	glm::mat4 trans1 = glm::translate(glm::mat4(1.0f), -pos);
	//paso 2
	glm::mat4 rotu = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), u);
	glm::mat4 rotv = glm::rotate(glm::mat4(1.0f), glm::radians(yoffset), v);
	//paso 3
	glm::mat4 trans2 = glm::translate(glm::mat4(1.0f), pos);
	//Composición de la matriz
	target = glm::vec3(trans2 * rotv * rotu * trans1 * glm::vec4(target, 1.0f));
	//pos = glm::vec3(trans2 * rotv* rotu * trans1 * glm::vec4(pos, 1.0f));
	//std::cout << "(" << target.x << "," << target.y << "," << target.z << ")" << std::endl;
}

void PAG::Camera::zoom(float yoffset)
{
	fovX -= yoffset;
}

float PAG::Camera::getPanAngle()
{
	return panAngle;
}

void PAG::Camera::reset()
{
	target = { 1.0f, 0.0f, 1.0f };
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
	return glm::perspective(getFovY(), wViewport / hViewport, zNear, zFar);
}

void PAG::Camera::setViewport(float w, float h)
{
	wViewport = w;
	hViewport = h;
}

PAG::Camera::~Camera()
{
	
}

