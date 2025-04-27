#pragma once
#include <SFML/Graphics.hpp>

class Vec2 {
public:
	float	x = 0.0f;
	float	y = 0.0f;
	
	Vec2();
	Vec2(float x, float y);
	Vec2(const sf::Vector2f& inst);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator * (const float rhs) const;
	Vec2 operator / (const float rhs) const;
	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;

	void operator *= (const float rhs);
	void operator /= (const float rhs);
	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);

	float dist(const Vec2& rhs) const;
	float distSqr(const Vec2& rhs) const;
	void normalize();
	float getMagnitude();
};