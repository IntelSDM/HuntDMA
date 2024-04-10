#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(float _x, float _y);
	~Vector2();

	float x, y;

	Vector2 operator *(Vector2 a);
	Vector2 operator /(Vector2 a);
	Vector2 operator +(Vector2 a);
	Vector2 operator -(Vector2 a);
	bool operator ==(Vector2 a);
	bool operator !=(Vector2 a);

	bool IsZero() const;

	static Vector2 Zero();

	static float Distance(Vector2 a, Vector2 b);
};

class Vector3
{
public:
	Vector3();
	Vector3(float _x, float _y, float _z);
	~Vector3();

	float x, y, z;

	Vector3 operator *(Vector3 a);
	Vector3 operator *(float f);
	Vector3 operator /(Vector3 a);
	Vector3 operator /(float f);
	Vector3 operator +(Vector3 a);
	Vector3 operator -(Vector3 a);
	bool operator ==(Vector3 a);
	bool operator !=(Vector3 a);

	bool IsZero() const;

	static float Dot(Vector3 left, Vector3 right);
	static float Distance(Vector3 a, Vector3 b);
	static int FormattedDistance(Vector3 a, Vector3 b);
	static Vector3 Zero();
	static Vector3 Lerp(Vector3 a, Vector3 b, float t);

	float Length() const;
	float LengthSqr() const;

	Vector3 Clamp() const;
};

struct ViewMatrix
{
public:
	float matrix[4][4];

	Vector3 Transform(const Vector3 vector) const;
};

class Vector4
{
public:
	Vector4();
	Vector4(float _x, float _y, float _z, float _w);
	~Vector4();

	float x, y, z, w;

	Vector4 operator*(const Vector4& a) const;
	Vector4 operator*(float f) const;

	Vector4 operator/(const Vector4& a) const;
	Vector4 operator/(float f) const;
	Vector4 operator+(const Vector4& a) const;
	Vector4 operator-(const Vector4& a) const;

	bool operator==(const Vector4& a) const;
	bool operator!=(const Vector4& a) const;

	bool IsZero() const;
	static Vector4 ToQuaternion(const Vector2& euler);
	static float Dot(const Vector4& left, const Vector4& right);
	static float Distance(const Vector4& a, const Vector4& b);
	static int FormattedDistance(const Vector4& a, const Vector4& b);
	static Vector4 Zero();
	static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);
	static Vector3 Mult(const Vector4& rotation, const Vector3& point);
	float Length() const;
	float LengthSqr() const;

	Vector4 Clamp() const;
};
