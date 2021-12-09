#pragma once

namespace PAG
{
	enum class MovType
	{
		LOCK,
		PAN,
		TILT,
		DOLLY,
		CRANE,
		ORBIT,
		ZOOM
	};

	class Camera
	{
		//glm::vec3 pos = { 4.0f, 1.0f, 4.0f };
		
		glm::vec3 pos = { 0.0f, 0.0f, 4.0f };
		glm::vec3 target = { 0.0f, 0.0f, 0.0f };
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		//ejes de la camara
		glm::vec3 n, v, u = {0,0,0};
		float fovX = 45.0f;
		float zNear =  0.1f;
		float zFar = 100.0f;
		float wViewport = 0.0f;
		float hViewport = 0.0f;
		float panAngle = 0.0f;
		float sensitivity = 0.1f;
		MovType movType = MovType::LOCK;
		void updateCameraAxis();
		float getFovY();
	public:
		Camera();
		Camera(float w, float h);
		Camera(glm::vec3 _pos, float _fovX, float _zn, float _zf, float _w, float _h);
		Camera& operator=(const Camera& orig) = default;
		~Camera();
		void setViewport(float w, float h);
		//Returns the camera direction. NORMALIZED.
		glm::vec3 cameraDirection(bool norm);
		glm::mat4 getViewMatrix();
		glm::mat4 getProjMatrix();
		glm::vec3 getUp();
		void setMov(MovType mov);
		void move(float xoffset, float yoffset);
		void move(int action);
		//Pan movement. Rotates the camera. The target changes.
		void pan(float offset);
		void tilt(float offset);
		void crane(int direction);
		void dolly(int direction);
		void orbit(float xoffset, float yoffset);
		void zoom(float yoffset);
		float getPanAngle();
		void reset();
	};
}

