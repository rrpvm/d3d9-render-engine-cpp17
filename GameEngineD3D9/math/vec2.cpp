#include "vec2.hpp"
inline vec2& vec2::operator+(const vec2& other)noexcept
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}
inline vec2& vec2::operator-(const vec2& other) noexcept
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}
inline vec2& vec2::operator*(const vec2& other) noexcept
{
    this->x *= other.x;
    this->y *= other.y;
    return *this;
}
inline vec2& vec2::operator/(const vec2& other) noexcept
{
    if (!other.x || !other.y)
    {
        vec2 tmp = vec2(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
        return tmp;
    }
    this->x /= other.x;
    this->y /= other.y;
    return *this;
}
inline vec2& vec2::operator+(const float& other) noexcept
{
    this->x += other;
    this->y += other;
    return *this;
}
inline vec2& vec2::operator-(const float& other) noexcept
{
    this->x -= other;
    this->y -= other;
    return *this;
}
inline vec2& vec2::operator*(const float& other) noexcept
{
    this->x *= other;
    this->y *= other;
    return *this;
}
inline vec2& vec2::operator/(const float& other)  noexcept
{
    if (!other)
    {
        vec2 tmp = vec2(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
        return tmp;
    }
    this->x /= other;
    this->y /= other;
    return *this;
}
inline float& vec2::dst(const vec2& other) const
{
    float length = sqrt((other.x - this->x) * (other.x - this->x) + (other.y - this->y) * (other.y - this->y));
    return length;
}
inline float& vec2::dot() const
{
    float d = this->x * this->x + this->y * this->y;
    return d;
}
inline float& vec2::length() const
{
    float length = sqrt(this->x * this->x + this->y * this->y);
}
vec2& vec2::operator=(const vec2& other) noexcept
{
    this->x = other.x;
    this->y = other.y;
    return *this;
}
