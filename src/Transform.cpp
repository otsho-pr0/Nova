#include <Nova/Transform.hpp>

Matrix Transform::projection;
Matrix Transform::view;

Transform::operator Matrix()
{
	return Matrix::Translate(position) * Matrix::RotateY(rotation.y) * Matrix::RotateX(rotation.x) * Matrix::RotateZ(rotation.z) * Matrix::Scale(scale);
}

void Transform::Update()
{
	rotation.x = Math::ClampAngle(rotation.x);
	rotation.y = Math::ClampAngle(rotation.y);
	rotation.z = Math::ClampAngle(rotation.z);
}

Matrix Transform::Basis()
{
    return Matrix::RotateY(rotation.y) * Matrix::RotateX(rotation.x) * Matrix::RotateZ(rotation.z);
}
