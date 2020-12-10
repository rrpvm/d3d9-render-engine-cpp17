#pragma once
class Color
{
	int r, g, b, a;
public:
	Color(int r = 0, int  g = 0, int b = 0, int a = 255)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	Color() :r(0), g(0), b(0), a(255) {};
	static const Color& black() {
		static const Color black_(0, 0, 0, 255);
		return black_;
	}
	static const Color& white() {
		static const Color tmp(255, 255, 255, 255);
		return tmp;
	}
	static const Color& red() {
		static const Color tmp(255, 0, 0, 255);
		return tmp;
	}
	static const Color& green() {
		static const Color tmp(0, 255, 0, 255);
		return tmp;
	}
	static const Color& blue() {
		static const Color tmp(0, 0, 255, 255);
		return tmp;
	}
	static const Color& purple() {
		static const Color tmp(255, 0, 255, 255);
		return tmp;
	}
	static const Color& yellow() {
		static const Color tmp(255, 255, 0, 255);
		return tmp;
	}
	static const Color& gray_m() {//mid
		static const Color tmp_(125, 125, 125);
		return tmp_;
	}
	static const Color& gray_b() {//black
		static const Color tmp_(65, 65, 65);
		return tmp_;
	}
	static const Color& gray_w() {//white
		static const Color tmp_(0.75 * 255, 0.75 * 255, 0.75 * 255);
		return tmp_;
	}
	void set_color(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	void set_color(int r, int g, int b, int a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	void set_alpha(int a)
	{
		this->a = a;
	}
	void clamp() {
		if (this->r > 255)this->r = 255;
		else if (this->r < 0)this->r = 0;
		if (this->g > 255)this->g = 255;
		else if (this->g < 0)this->g = 0;
		if (this->b > 255)this->b = 255;
		else if (this->b < 0)this->b = 0;
		if (this->a > 255)this->a = 255;
		else if (this->a < 0)this->a = 0;
		/*can be replaced by ternar operators: r > 255.f ? r = 255.f : r < 0.0f ? r = 0.0f : r = r*/
	}
	const unsigned long to_dword() const
	{
		return (unsigned long)(((this->a & 255) << 24) | ((this->r & 255) << 16) | ((this->g & 255) << 8) | ((this->b & 255)));
	}		
	Color& operator*=(int x)
	{
		this->r *= x;
		this->g *= x;
		this->b *= x;
		this->a *= x;
		this->clamp();
		return *this;
	}
	Color& operator*(const Color& x)
	{
		this->r *= x.r;
		this->g *= x.g;
		this->b *= x.b;
		this->a *= x.a;
		this->clamp();
		return *this;
	}
	Color& operator/(const Color& x)
	{
		this->r /= x.r;
		this->g /= x.g;
		this->b /= x.b;
		this->a /= x.a;
		this->clamp();
		return *this;
	}
	Color& operator-(const Color& x)
	{
		this->r -= x.r;
		this->g -= x.g;
		this->b -= x.b;
		this->a -= x.a;
		this->clamp();
		return *this;
	}
	Color& operator+(const Color& x)
	{
		this->r += x.r;
		this->g += x.g;
		this->b += x.b;
		this->a += x.a;
		this->clamp();
		return *this;
	}
	Color& operator=(const Color& a)
	{
		this->r = a.r;
		this->g = a.g;
		this->b = a.b;
		this->a = a.a;
		return *this;
	}
	int operator[](int& index)
	{
		assert(index > 0 && index <= 3 && "incorrect index");
		return *(int*)(reinterpret_cast<uintptr_t>(this) + 0x4 * index);
	}
private:
};
class ColorF
{
public:
	float r, g, b, a;

	ColorF(float r = 0.0f, float  g = 0, float b = 0, float a = 1.0f)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	static ColorF black() {
		static const ColorF black_ = ColorF(0.0f, 0.0f, 0.0f, 1.0f);
		return black_;
	}
	static ColorF white() {
		static const ColorF white_ = ColorF(1.0f, 1.0f, 1.0f, 1.0f);
		return white_;
	}
	static ColorF red() {
		static const ColorF red_ = ColorF(1.0f, 0.0f, 0.0f, 1.0f);
		return red_;
	}
	static ColorF green() {
		static const ColorF green_ = ColorF(0.0f, 1.0f, 0.0f, 1.0f);
		return green_;
	}
	void clamp() {
		if (this->r > 1.0f)this->r = 1.0f;
		else if (this->r < 0)this->r = 0;
		if (this->g > 1.0f)this->g = 1.0f;
		else if (this->g < 0)this->g = 0;
		if (this->b > 1.0f)this->b = 1.0f;
		else if (this->b < 0)this->b = 0;
		if (this->a > 1.0f)this->a = 1.0f;
		else if (this->a < 0)this->a = 0;
		/*can be replaced by ternar operators: r > 255.f ? r = 255.f : r < 0.0f ? r = 0.0f : r = r*/
	}
	static ColorF gray_m() {//mid
		static const ColorF gray_m_ = ColorF(0.5f, 0.5f, 0.5f);
		return gray_m_;
	}
	static ColorF gray_b() {//black
		static const ColorF gray_b_ = ColorF(0.25f, 0.25f, 0.25f);
		return gray_b_;
	}
	static ColorF gray_w() {//white
		static const ColorF gray_w_ = ColorF(0.75f, 0.75f, 0.75f);
		return gray_w_;
	}
	void set_color(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->clamp();
	}
	void set_color(int r, int g, int b) {
		this->r = static_cast<float>(r)/255.f;
		this->g = static_cast<float>(g)/255.f;
		this->b = static_cast<float>(b)/255.f;
		this->clamp();
	}
	void set_color(int r, int g, int b, int a) {
		this->r = static_cast<float>(r) / 255.f;
		this->g = static_cast<float>(g) / 255.f;
		this->b = static_cast<float>(b) / 255.f;
		this->a = static_cast<float>(a) / 255.f;
		this->clamp();
	}
	void set_color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		this->clamp();
	}
	void set_alpha(float a)
	{
		this->a = a;
	}
	ColorF& operator*=(float x)
	{
		this->r *= x;
		this->g *= x;
		this->b *= x;
		this->a *= x;
		this->clamp();
		return *this;
	}
	ColorF& operator*(const ColorF& x)
	{
		this->r *= x.r;
		this->g *= x.g;
		this->b *= x.b;
		this->a *= x.a;
		this->clamp();
		return *this;
	}
	ColorF& operator/(const ColorF& x)
	{
		this->r /= x.r;
		this->g /= x.g;
		this->b /= x.b;
		this->a /= x.a;
		this->clamp();
		return *this;
	}
	ColorF& operator-(const ColorF& x)
	{
		this->r -= x.r;
		this->g -= x.g;
		this->b -= x.b;
		this->a -= x.a;
		this->clamp();
		return *this;
	}
	ColorF& operator+(const ColorF& x)
	{
		this->r += x.r;
		this->g += x.g;
		this->b += x.b;
		this->a += x.a;
		this->clamp();
		return *this;
	}
	ColorF& operator=(const ColorF& a)
	{
		this->r = a.r;
		this->g = a.g;
		this->b = a.b;
		this->a = a.a;
		return *this;
	}
	//int& operator[](int index);

private:
};