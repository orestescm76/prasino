#pragma once

namespace PAG
{
	enum class MovType
	{
		PAN,
		TILT,
		DOLLY,
		CRANE,
		ORBIT,
		ZOOM
	};

	enum class Direction
	{
		RIGHT,
		LEFT
	};

	class Camera
	{
		glm::vec3 pos = { 3.0f, 1.0f, 3.0f };
		glm::vec3 orientation = { -2.0f, 0.0f, -2.0f };
		glm::vec3 target = { 1.0f, 1.0f, 1.0f };
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		glm::mat4 view = glm::mat4();
		glm::mat4 proj = glm::mat4();
		float fov = 45.0f;
		float zNear =0.1f;
		float zFar = 100.0f;
		float wViewport = 0.0f;
		float hViewport = 0.0f;
		float panAngle = 0.0f;
		MovType movType = MovType::PAN;
	public:
		Camera();
		Camera(float w, float h);
		Camera(glm::vec3 p, float a, float zn, float zf, float w, float h);
		~Camera();
		void setViewport(float w, float h);
		//Returns the camera direction. NORMALIZED.
		glm::vec3 cameraDirection(bool norm);
		glm::mat4 getViewMatrix();
		glm::mat4 getProjMatrix();
		glm::vec3 getUp();
		//Pan movement. Rotates the camera. The target changes.
		void pan(float degrees, Direction dir);
		float getPanAngle();
	};
}

