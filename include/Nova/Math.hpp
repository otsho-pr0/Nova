#pragma once

class Vector2
{
public:
	union
	{
		struct
		{
			float x;
			float y;
		};
		struct
		{
			float u;
			float v;
		};
		float f[2];
	};

	Vector2(float = 0.0f);
	Vector2(float x, float y);

	const float& operator[](unsigned char) const;
	float& operator[](unsigned char);

	Vector2 operator+(const Vector2&) const;
	Vector2& operator+=(const Vector2&);
	Vector2 operator-(const Vector2&) const;
	Vector2 operator-() const;
	Vector2& operator-=(const Vector2&);
	Vector2 operator*(const Vector2&) const;
	Vector2& operator*=(const Vector2&);
	Vector2 operator*(const float&) const;
	Vector2& operator*=(const float&);
	Vector2 operator/(const Vector2&) const;
	Vector2& operator/=(const Vector2&);
	Vector2 operator/(const float&) const;
	Vector2& operator/=(const float&);

	static float Dot(const Vector2&, const Vector2&);
	static float Cross(const Vector2&, const Vector2&);

	float Length() const;
	Vector2 Normalized() const;
	Vector2& Normalize();
};

class Vector3
{
public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		struct
		{
			float u;
			float v;
			float w;
		};
		struct
		{
			float r;
			float g;
			float b;
		};
		float f[3];
	};

	Vector3(float = 0.0f);
	Vector3(float x, float y, float z);
	Vector3(const Vector2& xy, float z);
	Vector3(float x, const Vector2& yz);

	const float& operator[](unsigned char) const;
	float& operator[](unsigned char);
	operator Vector2();

	float* operator&();
	Vector3 operator+(const Vector3&) const;
	Vector3& operator+=(const Vector3&);
	Vector3 operator-(const Vector3&) const;
	Vector3 operator-() const;
	Vector3& operator-=(const Vector3&);
	Vector3 operator*(const Vector3&) const;
	Vector3& operator*=(const Vector3&);
	Vector3 operator*(const float&) const;
	Vector3& operator*=(const float&);
	Vector3 operator/(const Vector3&) const;
	Vector3& operator/=(const Vector3&);
	Vector3 operator/(const float&) const;
	Vector3& operator/=(const float&);
	bool operator==(const Vector3&) const;

	static Vector3 Right;
	static Vector3 Left;
	static Vector3 Up;
	static Vector3 Down;
	static Vector3 Forward;
	static Vector3 Backward;

	static float Dot(const Vector3&, const Vector3&);
	static Vector3 Cross(const Vector3&, const Vector3&);

	float Length() const;
	Vector3 Normalized() const;
	Vector3& Normalize();
};

class Vector4
{
public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float f[4];
	};

	Vector4(float = 0.0f);
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector2& xy, float z, float w);
	Vector4(float x, float y, const Vector2& zw);
	Vector4(const Vector2& xy, const Vector2& zw);
	Vector4(const Vector3& xyz, float w);
	Vector4(float x, const Vector3& yzw);

	const float& operator[](unsigned char) const;
	float& operator[](unsigned char);
	operator Vector3();

	float* operator&();
	Vector4 operator+(const Vector4&) const;
	Vector4& operator+=(const Vector4&);
	Vector4 operator-(const Vector4&) const;
	Vector4 operator-() const;
	Vector4& operator-=(const Vector4&);
	Vector4 operator*(const Vector4&) const;
	Vector4& operator*=(const Vector4&);
	Vector4 operator*(const float&) const;
	Vector4& operator*=(const float&);
	Vector4 operator/(const Vector4&) const;
	Vector4& operator/=(const Vector4&);
	Vector4 operator/(const float&) const;
	Vector4& operator/=(const float&);

	static float Dot(const Vector4&, const Vector4&);

	float Length() const;
	Vector4 Normalized() const;
	Vector4& Normalize();
};

class Matrix
{
public:
	Matrix() = default;
	Matrix(float);

	const Vector4& operator[](unsigned char) const;
	Vector4& operator[](unsigned char);

	Matrix operator*(const Matrix&) const;
	Matrix& operator*=(const Matrix&);
	Vector4 operator*(const Vector4&);
	Vector3 operator*(const Vector3&);

	static Matrix Perspective(float fov, float aspect, float near, float far);
	static Matrix Orthographic(float fov, float aspect, float near, float far);
	static Matrix LookAt(const Vector3& position, const Vector3& target, const Vector3& worldUp);
	static Matrix Translate(float x, float y, float z);
	static Matrix Translate(const Vector3&);
	static Matrix RotateX(float);
	static Matrix RotateY(float);
	static Matrix RotateZ(float);
	static Matrix Scale(float x, float y, float z);
	static Matrix Scale(const Vector3&);

	Matrix Transposed() const;
	Matrix& Transpose();

private:
	Vector4 v[4];
};

namespace Math
{
	constexpr float PI = 3.141592653589793f;
	constexpr float TwoPI = 6.283185307179586f;
	constexpr float HalfPI = 1.570796326794897f;
	constexpr float BreakPoint = 0.0001f;

	float Abs(float x);
	float DegToRad(float x);
	float RadToDeg(float x);
	float Min(float a, float b);
	float Max(float a, float b);
	float Lerp(float from, float to, float weight);
	float LerpAngle(float from, float to, float weight);
	float Clamp(float x, float min, float max);
	float ClampAngle(float a);
}