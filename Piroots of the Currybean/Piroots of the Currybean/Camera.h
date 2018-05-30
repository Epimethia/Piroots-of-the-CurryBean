#pragma once
#include "Resource.h"

class Camera {
public:
	~Camera();
	static std::shared_ptr<Camera> GetInstance();
	static void DestroyInstance();
	static glm::mat4& GetMatrix();
	static glm::vec3& GetPos() { return CameraPos; };
	static void SetCameraPos(glm::vec3 _SnapToPos);
	static int XRot;
	static int YRot;

private:
	Camera();
	static std::shared_ptr<Camera> CameraPtr;
	static glm::mat4 View, Projection;
	static glm::mat4 VPMat;
	static glm::vec3 CameraPos;
};