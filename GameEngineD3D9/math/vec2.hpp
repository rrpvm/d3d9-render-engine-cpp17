#pragma once
#include <limits>
class vec2
{
public:
	float x;
	float y;
	vec2() { this->x = 0.0f; this->y = 0.0f; };
	vec2(float X, float Y) :x(X), y(Y) {};
	inline vec2& operator+(const vec2& other)  noexcept;
	inline vec2& operator-(const vec2& other)  noexcept;
	inline vec2& operator*(const vec2& other)  noexcept;
	inline vec2& operator/(const vec2& other)  noexcept;
	inline vec2& operator+(const float& other)  noexcept;
	inline vec2& operator-(const float& other)  noexcept;
	inline vec2& operator*(const float& other)  noexcept;
	inline vec2& operator/(const float& other)  noexcept;
	inline float& dst(const vec2& other) const;//dst means distance
	inline float& dot() const;
	inline float& length() const;
	vec2& operator=(const vec2& other) noexcept;
private:
};

