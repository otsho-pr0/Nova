#include <Nova/Math.hpp>

#include <cmath>
#include <iostream>

Vector2::Vector2(float a) :
	x(a),
	y(a)
{
}

Vector2::Vector2(float x, float y) :
	x(x),
	y(y)
{
}

const float& Vector2::operator[](unsigned char i) const
{
	return f[i];
}

float& Vector2::operator[](unsigned char i)
{
	return f[i];
}

Vector2 Vector2::operator+(const Vector2& o) const
{
	return { x + o.x, y + o.y };
}

Vector2& Vector2::operator+=(const Vector2& o)
{
	x += o.x;
	y += o.y;
	return *this;
}

Vector2 Vector2::operator-(const Vector2& o) const
{
	return { x - o.x, y - o.y };
}

Vector2& Vector2::operator-=(const Vector2& o)
{
	x -= o.x;
	y -= o.y;
	return *this;
}

Vector2 Vector2::operator*(const Vector2& o) const
{
	return { x * o.x, y * o.y };
}

Vector2& Vector2::operator*=(const Vector2& o)
{
	x *= o.x;
	y *= o.y;
	return *this;
}

Vector2 Vector2::operator*(const float& a) const
{
	return { x * a, y * a };
}

Vector2& Vector2::operator*=(const float& a)
{
	x *= a;
	y *= a;
	return *this;
}

Vector2 Vector2::operator/(const Vector2& o) const
{
	return { x / o.x, y / o.y };
}

Vector2& Vector2::operator/=(const Vector2& o)
{
	x /= o.x;
	y /= o.y;
	return *this;
}

Vector2 Vector2::operator/(const float& a) const
{
	return { x / a, y / a };
}

Vector2& Vector2::operator/=(const float& a)
{
	x /= a;
	y /= a;
	return *this;
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float Vector2::Cross(const Vector2& a, const Vector2& b)
{
	return (a.x * b.y) - (a.y * b.x);
}

float Vector2::Length() const
{
	return std::sqrtf(x * x + y * y);
}

Vector2 Vector2::Normalized() const
{
	float length = Length();
	return { x / length, y / length };
}

Vector2& Vector2::Normalize()
{
	float length = Length();
	x /= length;
	y /= length;
	return *this;
}

Vector3 Vector3::Right    = {  1.0f,  0.0f,  0.0f };
Vector3 Vector3::Left     = { -1.0f,  0.0f,  0.0f };
Vector3 Vector3::Up       = {  0.0f,  1.0f,  0.0f };
Vector3 Vector3::Down     = {  0.0f, -1.0f,  0.0f };
Vector3 Vector3::Forward  = {  0.0f,  0.0f,  1.0f };
Vector3 Vector3::Backward = {  0.0f,  0.0f, -1.0f };

Vector3::Vector3(float a) :
	x(a),
	y(a),
	z(a)
{
}

Vector3::Vector3(float x, float y, float z) :
	x(x),
	y(y),
	z(z)
{
}

Vector3::Vector3(const Vector2& xy, float z) :
	x(xy.x),
	y(xy.y),
	z(z)
{
}

Vector3::Vector3(float x, const Vector2& yz) :
	x(x),
	y(yz.x),
	z(yz.y)
{
}

const float& Vector3::operator[](unsigned char i) const
{
	return f[i];
}

float& Vector3::operator[](unsigned char i)
{
	return f[i];
}

Vector3::operator Vector2()
{
	return { x, y };
}

float* Vector3::operator&()
{
	return &x;
}

Vector3 Vector3::operator+(const Vector3& o) const
{
	return { x + o.x, y + o.y, z + o.z };
}

Vector3& Vector3::operator+=(const Vector3& o)
{
	x += o.x;
	y += o.y;
	z += o.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& o) const
{
	return { x - o.x, y - o.y, z - o.z };
}

Vector3& Vector3::operator-=(const Vector3& o)
{
	x -= o.x;
	y -= o.y;
	z -= o.z;
	return *this;
}

Vector3 Vector3::operator*(const Vector3& o) const
{
	return { x * o.x, y * o.y, z * o.z };
}

Vector3& Vector3::operator*=(const Vector3& o)
{
	x *= o.x;
	y *= o.y;
	z *= o.z;
	return *this;
}

Vector3 Vector3::operator*(const float& a) const
{
	return { x * a, y * a, z * a };
}

Vector3& Vector3::operator*=(const float& a)
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

Vector3 Vector3::operator/(const Vector3& o) const
{
	return { x / o.x, y / o.y, z / o.z };
}

Vector3& Vector3::operator/=(const Vector3& o)
{
	x /= o.x;
	y /= o.y;
	z /= o.z;
	return *this;
}

Vector3 Vector3::operator/(const float& a) const
{
	return { x / a, y / a, z / a };
}

Vector3& Vector3::operator/=(const float& a)
{
	x /= a;
	y /= a;
	z /= a;
	return *this;
}

bool Vector3::operator==(const Vector3& o) const
{
	return x == o.x && y == o.y && z == o.z;
}

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
	return { a.y * b.z - a.z * b.y, -(a.x * b.z - a.z * b.x), a.x * b.y - a.y * b.x	};
}

float Vector3::Length() const
{
	return std::sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::Normalized() const
{
	float length = Length();
	if (length)
		return { x / length, y / length, z / length };
	else
		return {};
}

Vector3& Vector3::Normalize()
{
	float length = Math::Max(Length(), 0.0f);
	if (length)
	{
		x /= length;
		y /= length;
		z /= length;
	}
	return *this;
}

Vector4::Vector4(float a) :
	x(a),
	y(a),
	z(a),
	w(a)
{
}

Vector4::Vector4(float x, float y, float z, float w) :
	x(x),
	y(y),
	z(z),
	w(w)
{
}

Vector4::Vector4(const Vector2& xy, float z, float w) :
	x(xy.x),
	y(xy.y),
	z(z),
	w(w)
{
}

Vector4::Vector4(float x, float y, const Vector2& zw) :
	x(x),
	y(y),
	z(zw.x),
	w(zw.y)
{
}

Vector4::Vector4(const Vector2& xy, const Vector2& zw) :
	x(xy.x),
	y(xy.y),
	z(zw.x),
	w(zw.y)
{
}

Vector4::Vector4(const Vector3& xyz, float w) :
	x(xyz.x),
	y(xyz.y),
	z(xyz.z),
	w(w)
{
}

Vector4::Vector4(float x, const Vector3& yzw) :
	x(x),
	y(yzw.y),
	z(yzw.z),
	w(yzw.w)
{
}

const float& Vector4::operator[](unsigned char i) const
{
	return f[i];
}

float& Vector4::operator[](unsigned char i)
{
	return f[i];
}

Vector4::operator Vector3()
{
	return { x, y, z };
}

float* Vector4::operator&()
{
	return &x;
}

Vector4 Vector4::operator+(const Vector4& o) const
{
	return { x + o.x, y + o.y, z + o.z, w + o.w };
}

Vector4& Vector4::operator+=(const Vector4& o)
{
	x += o.x;
	y += o.y;
	z += o.z;
	w += o.w;
	return *this;
}

Vector4 Vector4::operator-(const Vector4& o) const
{
	return { x - o.x, y - o.y, z - o.z, w - o.w };
}

Vector4& Vector4::operator-=(const Vector4& o)
{
	x -= o.x;
	y -= o.y;
	z -= o.z;
	w -= o.w;
	return *this;
}

Vector4 Vector4::operator*(const Vector4& o) const
{
	return { x * o.x, y * o.y, z * o.z, w * o.w };
}

Vector4& Vector4::operator*=(const Vector4& o)
{
	x *= o.x;
	y *= o.y;
	z *= o.z;
	w *= o.w;
	return *this;
}

Vector4 Vector4::operator*(const float& a) const
{
	return { x * a, y * a, z * a, w * a };
}

Vector4& Vector4::operator*=(const float& a)
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;
	return *this;
}

Vector4 Vector4::operator/(const Vector4& o) const
{
	return { x / o.x, y / o.y, z / o.z, w / o.w };
}

Vector4& Vector4::operator/=(const Vector4& o)
{
	x /= o.x;
	y /= o.y;
	z /= o.z;
	w /= o.w;
	return *this;
}

Vector4 Vector4::operator/(const float& a) const
{
	return { x / a, y / a, z / a, w / a };
}

Vector4& Vector4::operator/=(const float& a)
{
	x /= a;
	y /= a;
	z /= a;
	w /= a;
	return *this;
}

float Vector4::Dot(const Vector4& a, const Vector4& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

float Vector4::Length() const
{
	return std::sqrtf(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::Normalized() const
{
	float length = Length();
	return { x / length, y / length, z / length, w / length };
}

Vector4& Vector4::Normalize()
{
	float length = Length();
	x /= length;
	y /= length;
	z /= length;
	w /= length;
	return *this;
}

Matrix::Matrix(float a)
{
	for (unsigned char i = 0; i < 4; i++)
		v[i][i] = a;
}

const Vector4& Matrix::operator[](unsigned char i) const
{
	return v[i];
}

Vector4& Matrix::operator[](unsigned char i)
{
	return v[i];
}

Matrix Matrix::operator*(const Matrix& o) const
{
	Matrix r;
	for (unsigned char j = 0; j < 4; j++)
		for (unsigned char i = 0; i < 4; i++)
			for (unsigned char k = 0; k < 4; k++)
				r[i][j] += v[i][k] * o[k][j];
	return r;
}

Matrix& Matrix::operator*=(const Matrix& o)
{
	Matrix r = *this;
	for (unsigned char j = 0; j < 4; j++)
		for (unsigned char i = 0; i < 4; i++)
			for (unsigned char k = 0; k < 4; k++)
				r[i][j] += v[i][k] * o[k][j];
	return *this;
}

Vector4 Matrix::operator*(const Vector4& o)
{
	Vector4 r;
	for (unsigned char i = 0; i < 4; i++)
		for (unsigned char j = 0; j < 4; j++)
				r[i] += v[i][j] * o[j];
	return r;
}

Vector3 Matrix::operator*(const Vector3& o)
{
	return operator*({ o, 1.0f });
}

Matrix Matrix::Perspective(float fov, float aspect, float near, float far)
{
	Matrix r;
	float h = 1.0f / std::tan(fov * 0.5f);
	float w = h / aspect;
	float range = far - near;
	r[0][0] = w;
	r[1][1] = h;
	r[2] = { 0.0f, 0.0f, far / range, -(far * near) / range };
	r[3][2] = 1.0f;
	return r;
}

Matrix Matrix::LookAt(const Vector3& position, const Vector3& target, const Vector3& worldUp)
{
	Matrix r;
	Vector3 forward = (target - position).Normalize();
	Vector3 right = Vector3::Cross(worldUp, forward);
	Vector3 up = Vector3::Cross(forward, right);
	r[0] = {   right.x,   right.y,   right.z, -Vector3::Dot(right,   position) };
	r[1] = {      up.x,      up.y,      up.z, -Vector3::Dot(up,      position) };
	r[2] = { forward.x, forward.y, forward.z, -Vector3::Dot(forward, position) };
	r[3] = {      0.0f,      0.0f,      0.0f,                             1.0f };
	return r;
}

Matrix Matrix::Translate(float x, float y, float z)
{
	Matrix r(1.0f);
	r[0][3] = x;
	r[1][3] = y;
	r[2][3] = z;
	return r;
}

Matrix Matrix::Translate(const Vector3& v)
{
	Matrix r(1.0f);
	r[0][3] = v.x;
	r[1][3] = v.y;
	r[2][3] = v.z;
	return r;
}

Matrix Matrix::RotateX(float a)
{
	Matrix r(1.0f);
	r[1][1] = std::cosf(a);
	r[2][1] = std::sinf(a);
	r[1][2] = -std::sinf(a);
	r[2][2] = std::cosf(a);
	return r;
}

Matrix Matrix::RotateY(float a)
{
	Matrix r(1.0f);
	r[0][0] = std::cosf(a);
	r[2][0] = -std::sinf(a);
	r[0][2] = std::sinf(a);
	r[2][2] = std::cosf(a);
	return r;
}

Matrix Matrix::RotateZ(float a)
{
	Matrix r(1.0f);
	r[0][0] = std::cosf(a);
	r[1][0] = std::sinf(a);
	r[0][1] = -std::sinf(a);
	r[1][1] = std::cosf(a);
	return r;
}

Matrix Matrix::Scale(float x, float y, float z)
{
	Matrix r(1.0f);
	r[0][0] = x;
	r[1][1] = y;
	r[2][2] = z;
	return r;
}

Matrix Matrix::Scale(const Vector3& v)
{
	Matrix r(1.0f);
	r[0][0] = v.x;
	r[1][1] = v.y;
	r[2][2] = v.z;
	return r;
}

Matrix Matrix::Transposed() const
{
	Matrix r;
	for (unsigned char j = 0; j < 4; j++)
		for (unsigned char i = 0; i < 4; i++)
			r[i][j] = v[j][i];
	return r;
}

Matrix& Matrix::Transpose()
{
	Matrix r = *this;
	for (unsigned char j = 0; j < 4; j++)
		for (unsigned char i = 0; i < 4; i++)
			v[i][j] = r[j][i];
	return *this;
}

float Math::Abs(float x)
{
    return std::abs(x);
}

float Math::DegToRad(float x)
{
	return x * (PI / 180.0f);
}

float Math::RadToDeg(float x)
{
	return x * (180.0f / PI);
}

float Math::Min(float a, float b)
{
	return (a < b) ? a : b;
}

float Math::Max(float a, float b)
{
	return (a > b) ? a : b;
}

float Math::Lerp(float from, float to, float weight)
{
	if (std::abs(to - from) <= BreakPoint)
		return to;
	return from + (to - from) * weight;
}

float Math::LerpAngle(float from, float to, float weight)
{
	if (std::abs(Math::ClampAngle(to - from)) <= BreakPoint)
		return to;
	return from + Math::ClampAngle(to - from) * weight;
}

float Math::Clamp(float x, float min, float max)
{
	return Min(Max(x, min), max);
}

float Math::ClampAngle(float a)
{
	float result = std::fmod(a, TwoPI);
	if (result < 0.0f)
		result += TwoPI;
	if (result >= PI)
		result -= TwoPI;
	return result;
}
