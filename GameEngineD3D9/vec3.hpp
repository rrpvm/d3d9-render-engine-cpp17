#pragma once
#include <limits>
class vec3
{
public:
	float x;
	float y;
	float z;
	vec3() { this->x = 0.0f; this->y = 0.0f; this->z = 0.0f; };
	vec3(float X, float Y, float Z) :x(X), y(Y),z(Z) {};
	inline vec3& operator+(const vec3& other)  noexcept;
	inline vec3& operator-(const vec3& other)  noexcept;
	inline vec3& operator*(const vec3& other)  noexcept;
	inline vec3& operator/(const vec3& other)  noexcept;
	inline vec3& operator+(const float& other)  noexcept;
	inline vec3& operator-(const float& other)  noexcept;
	inline vec3& operator*(const float& other)  noexcept;
	inline vec3& operator/(const float& other)  noexcept;
	inline float& dst(const vec3& other) const;//dst means distance
	inline float& dot() const;
	inline float& length() const;
	vec3& operator=(const vec3& other) noexcept;
private:
};

