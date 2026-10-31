#pragma once

#include <Nova/Node.hpp>

class Camera : public Node
{
public:
	Camera();

	float fov = Math::DegToRad(50.0f);
	float aspect = 16.0f/9.0f;
	float near = 0.0f;
	float far = 100.0f;

	void Update(Matrix parent = Matrix(1.0f)) override;
};