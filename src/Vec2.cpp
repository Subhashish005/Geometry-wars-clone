#include "Vec2.h"
#include <cmath>

Vec2::Vec2() :
	x(0.0f),
	y(0.0f)
{}

Vec2::Vec2(float x, float y) :
	x(x),
	y(y) 
{}

Vec2::Vec2(const sf::Vector2f& rhs) :
	x(rhs.x),
	y(rhs.y)
{}

bool Vec2::operator == (const Vec2& rhs) const {
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const {
	return !(x == rhs.x && y == rhs.y);
}

Vec2 Vec2::operator * (const float rhs) const {
	return Vec2(x * rhs, y * rhs);
}

Vec2 Vec2::operator / (const float rhs) const {
	return Vec2(x / rhs, y / rhs);
}

Vec2 Vec2::operator + (const Vec2& rhs) const {
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const {
	return Vec2(x - rhs.x, y - rhs.y);
}

void Vec2::operator *= (const float rhs) {
	x *= rhs;
	y *= rhs;
}

void Vec2::operator /= (const float rhs) {
	x /= rhs;
	y /= rhs;
}

void Vec2::operator += (const Vec2& rhs) {
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs) {
	x -= rhs.x;
	y -= rhs.y;
}

float Vec2::dist(const Vec2& rhs) const {
	return std::sqrtf(std::powf(x - rhs.x, 2) + std::powf(y - rhs.y, 2));
}

float Vec2::distSqr(const Vec2& rhs) const {
	return std::powf(x - rhs.x, 2) + std::powf(y - rhs.y, 2);
}

void Vec2::normalize() {
	float magnitude = std::sqrtf(std::powf(x, 2) + std::powf(y, 2));
	
	x /= magnitude;
	y /= magnitude;
}

float Vec2::getMagnitude() {
	return std::sqrtf(std::powf(x, 2) + std::powf(y, 2));
}