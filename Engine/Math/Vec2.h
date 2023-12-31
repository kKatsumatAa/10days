#pragma once

class Vec2
{
public:
	float x;
	float y;

public:
	//RXgN^
	Vec2()
		:x(0), y(0)
	{}

	Vec2(float x, float y)
		:x(x), y(y)
	{}

	//oÖ
	float GetLength() const;
	void  Normalized();
	Vec2  GetNormalize() const;
	float Dot(const Vec2& v) const;
	float Cross(const Vec2& v) const;

	Vec2  operator -()const;//vec¯mÌø«Z
	Vec2& operator -=(const Vec2& other);//¡Zq(-)

	Vec2  operator +()const;
	Vec2& operator +=(const Vec2& other);//¡Zq(+)

	Vec2  operator *(float other);
	Vec2& operator *=(float other);//¡Zq(*)

	Vec2  operator /(float other);
	Vec2& operator /=(float other);//¡Zq(/)
};

//ñZqI[o[[h
const Vec2 operator +(const Vec2& v1, const Vec2& v2);
const Vec2 operator -(const Vec2& v1, const Vec2& v2);
const Vec2 operator *(const Vec2& v, float s);
const Vec2 operator *(float s, const Vec2& v);
const Vec2 operator /(const Vec2& v, float s);