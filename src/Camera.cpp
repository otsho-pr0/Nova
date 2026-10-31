#include <Nova/Camera.hpp>

Camera::Camera()
{
	Node::name = "Camera";
}

void Camera::Update(Matrix parent)
{
	Node::Update();
	Transform::view = Matrix::LookAt(transform.position, transform.position + transform.Basis() * Vector3::Forward, transform.Basis() * Vector3::Up);
	Transform::projection = Matrix::Perspective(fov, aspect, near, far);
}
