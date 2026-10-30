#pragma once

#include <Nova/Node.hpp>

class Camera : public Node
{
public:
	float fov;
	float aspect;
	float near;
	float far;

	void Update(Matrix parent = Matrix(1.0f)) override;
};