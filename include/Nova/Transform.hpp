#pragma once

#include <Nova/Math.hpp>

struct Transform
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale{ 1.0f };

	operator Matrix();

	void Update();
	Matrix Basis();

	static Matrix projection;
	static Matrix view;
};