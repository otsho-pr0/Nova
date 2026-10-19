#pragma once

#include <Nova/Transform.hpp>

class Camera
{
public:
	Transform transform;

	float fov;
	float aspect;
	float near;
	float far;

	void Update();
};