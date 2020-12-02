#include "vec3.hpp"
inline vec3& vec3::operator+(const vec3& other)noexcept
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}
inline vec3& vec3::operator-(const vec3& other) noexcept
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}
inline vec3& vec3::operator*(const vec3& other) noexcept
{
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return *this;
}
inline vec3& vec3::operator/(const vec3& other) noexcept
{
    if (!other.x || !other.y || !other.z)
    {
        vec3 tmp = vec3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
        return tmp;
    }
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return *this;
}
inline vec3& vec3::operator+(const float& other) noexcept
{
    this->x += other;
    this->y += other;
    this->z += other;
    return *this;
}
inline vec3& vec3::operator-(const float& other) noexcept
{
    this->x -= other;
    this->y -= other;
    this->z -= other;
    return *this;
}
inline vec3& vec3::operator*(const float& other) noexcept
{
    this->x *= other;
    this->y *= other;
    this->z *= other;
    return *this;
}
inline vec3& vec3::operator/(const float& other)  noexcept
{
    if (!other)
    {
        vec3 tmp = vec3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
        return tmp;
    }
    this->x /= other;
    this->y /= other;
    this->z /= other;
    return *this;
}
inline float& vec3::dst(const vec3& other) const
{
    float length = sqrt((other.x - this->x) * (other.x - this->x) + (other.y - this->y) * (other.y - this->y) + (other.z - this->z) * (other.z- this->z));
    return length;
}
inline float& vec3::dot() const
{
    float d = this->x * this->x + this->y * this->y + this->z * this->z;
    return d;
}
inline float& vec3::length() const
{
    float length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}
vec3& vec3::operator=(const vec3& other) noexcept
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}
