#include <Nova/Camera.hpp>
#include <iostream>

void Camera::Update()
{
	transform.Update();
	Transform::view       = Matrix::LookAt(transform.position, transform.position + transform.Basis() * Vector3::Forward, transform.Basis() * Vector3::Up);
	Transform::projection = Matrix::Perspective(fov, aspect, near, far);
}
