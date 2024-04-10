#include "pch.h"
#include "vector.h"

#include <cmath>
#include <emmintrin.h>

Vector2::Vector2() : x(0), y(0) { }

Vector2::Vector2(float _x, float _y) : x(_x), y(_y) { }

Vector2::~Vector2() { }

Vector2 Vector2::operator*(Vector2 a)
{
    return Vector2(x * a.x, y * a.y);
}

Vector2 Vector2::operator/(Vector2 a)
{
    return Vector2(x / a.x, y / a.y);
}

Vector2 Vector2::operator+(Vector2 a)
{
    return Vector2(x + a.x, y + a.y);
}

Vector2 Vector2::operator-(Vector2 a)
{
    return Vector2(x - a.x, y - a.y);
}

bool Vector2::operator==(Vector2 a)
{
    return x == a.x && y == a.y;
}

bool Vector2::operator!=(Vector2 a)
{
    return x != a.x || y != a.y;
}

bool Vector2::IsZero() const
{
    return !x && !y;
}

Vector2 Vector2::Zero()
{
    return Vector2();
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
    const auto difference = Vector2(
        a.x - b.x,
        a.y - b.y);

    return sqrt(
        powf(difference.x, 2.0f) +
        powf(difference.y, 2.0f));
}

Vector3::Vector3() : x(0), y(0), z(0) { }

Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

Vector3::~Vector3() { }

Vector3 Vector3::operator*(Vector3 a)
{
    return Vector3(x * a.x, y * a.y, z * a.z);
}

Vector3 Vector3::operator*(float f)
{
    return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator/(Vector3 a)
{
    return Vector3(x / a.x, y / a.y, z / a.z);
}

Vector3 Vector3::operator/(float f)
{
    return Vector3(x / f, y / f, z / f);
}

Vector3 Vector3::operator+(Vector3 a)
{
    return Vector3(x + a.x, y + a.y, z + a.z);
}

Vector3 Vector3::operator-(Vector3 a)
{
    return Vector3(x - a.x, y - a.y, z - a.z);
}

bool Vector3::operator==(Vector3 a)
{
    return x == a.x && y == a.y && z == a.z;
}

bool Vector3::operator!=(Vector3 a)
{
    return x != a.x || y != a.y || z != a.z;
}

bool Vector3::IsZero() const
{
    return !x && !y && !z;
}

float Vector3::Dot(Vector3 left, Vector3 right)
{
    return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}

float Vector3::Distance(Vector3 a, Vector3 b)
{
    const auto difference = Vector3(
        a.x - b.x,
        a.y - b.y,
        a.z - b.z);

    return sqrt(
        powf(difference.x, 2.0f) +
        powf(difference.y, 2.0f) +
        powf(difference.z, 2.0f)
    );

}

int Vector3::FormattedDistance(Vector3 a, Vector3 b)
{
    const auto difference = Vector3(
        a.x - b.x,
        a.y - b.y,
        a.z - b.z);

    int distance = sqrt(
        powf(difference.x, 2.0f) +
        powf(difference.y, 2.0f) +
        powf(difference.z, 2.0f)
    );
    return (int)(5 * round(distance / 5.0));
}

Vector3 Vector3::Zero()
{
    return Vector3();
}

float Vector3::Length() const
{
    return sqrt((x * x) + (y * y) + (z * z));
}

float Vector3::LengthSqr() const
{
    return (x * x) + (y * y) + (z * z);
}

Vector3 Vector3::Clamp() const
{
    Vector3 angles = *this;

    if (angles.x > 89.f)
        angles.x -= 360.f;
    else if (angles.x < -89.f)
        angles.x += 360.f;

    if (angles.y > 180.f)
        angles.y -= 360.f;
    else if (angles.y < -180.f)
        angles.y += 360.f;

    angles.z = 0.f;
    return angles;
}

Vector3 Vector3::Lerp(Vector3 a, Vector3 b, float t)
{
    return a * (1.f - t) + b * t;
}


Vector3 ViewMatrix::Transform(const Vector3 vector) const
{
    Vector3 transformed;

    transformed.x = vector.y * matrix[0][1] + vector.x * matrix[0][0] + vector.z * matrix[0][2] + matrix[0][3];
    transformed.y = vector.y * matrix[1][1] + vector.x * matrix[1][0] + vector.z * matrix[1][2] + matrix[1][3];
    transformed.z = vector.y * matrix[3][1] + vector.x * matrix[3][0] + vector.z * matrix[3][2] + matrix[3][3];

    return transformed;
}

Vector4::Vector4() : x(0), y(0), z(0), w(0)
{
}

Vector4::Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
{
}

Vector4::~Vector4()
{
}

Vector4 Vector4::operator*(const Vector4& a) const
{
	return Vector4(x * a.x, y * a.y, z * a.z, w * a.w);
}

Vector3 Vector4::Mult(const Vector4& rotation, const Vector3& point)
{
	float num = rotation.x * 2.0f;
	float num2 = rotation.y * 2.f;
	float num3 = rotation.z * 2.f;
	float num4 = rotation.x * num;
	float num5 = rotation.y * num2;
	float num6 = rotation.z * num3;
	float num7 = rotation.x * num2;
	float num8 = rotation.x * num3;
	float num9 = rotation.y * num3;
	float num10 = rotation.w * num;
	float num11 = rotation.w * num2;
	float num12 = rotation.w * num3;
	Vector3 result;
	result.x = (1.f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
	result.y = (num7 + num12) * point.x + (1.0f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
	result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.f - (num4 + num5)) * point.z;
	return result;

}


Vector4 Vector4::operator*(float f) const
{
	return Vector4(x * f, y * f, z * f, w * f);
}

Vector4 Vector4::operator/(const Vector4& a) const
{
	return Vector4(x / a.x, y / a.y, z / a.z, w / a.w);
}

Vector4 Vector4::operator/(float f) const
{
	return Vector4(x / f, y / f, z / f, w / f);
}

Vector4 Vector4::operator+(const Vector4& a) const
{
	return Vector4(x + a.x, y + a.y, z + a.z, w + a.w);
}

Vector4 Vector4::operator-(const Vector4& a) const
{
	return Vector4(x - a.x, y - a.y, z - a.z, w - a.w);
}

bool Vector4::operator==(const Vector4& a) const
{
	return x == a.x && y == a.y && z == a.z && w == a.w;
}

bool Vector4::operator!=(const Vector4& a) const
{
	return x != a.x || y != a.y || z != a.z || w != a.w;
}

bool Vector4::IsZero() const
{
	return !x && !y && !z && !w;
}

float Vector4::Dot(const Vector4& left, const Vector4& right)
{
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w);
}

float Vector4::Distance(const Vector4& a, const Vector4& b)
{
	const Vector4 difference = Vector4(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w);

	return sqrtf(
		powf(difference.x, 2.0f) +
		powf(difference.y, 2.0f) +
		powf(difference.z, 2.0f) +
		powf(difference.w, 2.0f)
	);
}

int Vector4::FormattedDistance(const Vector4& a, const Vector4& b)
{
	const auto difference = Vector4(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w);

	int distance = sqrt(
		powf(difference.x, 2.0f) +
		powf(difference.y, 2.0f) +
		powf(difference.z, 2.0f) +
		powf(difference.w, 2.0f)
	);
	return (int)(5 * round(distance / 5.0));
}

Vector4 Vector4::Zero()
{
	return Vector4();
}

float Vector4::Length() const
{
	return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

float Vector4::LengthSqr() const
{
	return (x * x) + (y * y) + (z * z) + (w * w);
}

Vector4 Vector4::Clamp() const
{
	Vector4 angles = *this;

	if (angles.x > 89.f)
		angles.x -= 360.f;
	else if (angles.x < -89.f)
		angles.x += 360.f;

	if (angles.y > 180.f)
		angles.y -= 360.f;
	else if (angles.y < -180.f)
		angles.y += 360.f;

	angles.z = 0.f;
	angles.w = 0.f;
	return angles;
}

Vector4 Vector4::ToQuaternion(const Vector2& euler)
{
	const auto to_radians = [](float degree) -> float
		{
			constexpr float pi = 3.14159265358979323846f;
			return degree * (pi / 180.f);
		};

	float heading = to_radians(euler.x);
	float attitude = to_radians(euler.y);
	float bank = to_radians(0);

	float c1 = cos(heading * 0.5f);
	float s1 = sin(heading * 0.5f);
	float c2 = cos(attitude * 0.5f);
	float s2 = sin(attitude * 0.5f);
	float c3 = cos(bank * 0.5f);
	float s3 = sin(bank * 0.5f);

	float c1c2 = c1 * c2;
	float s1s2 = s1 * s2;

	Vector4 quat{
		c1c2 * s3 + s1s2 * c3,
		s1 * c2 * c3 + c1 * s2 * s3,
		c1 * s2 * c3 - s1 * c2 * s3,
		c1c2 * c3 - s1s2 * s3
	};

	return { quat.y, quat.z, (quat.x * -1.f), quat.w };
}
