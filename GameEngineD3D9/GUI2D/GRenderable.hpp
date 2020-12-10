#pragma once

class GRenderable //using by  GuiDrawManager
{
public:
	GRenderable() {
		position = vec2(-1, -1);
		size = vec2(-1, -1);
		radius = -1.0f;
	}
	vec2 position;
	vec2 size;
	float radius;
	virtual void draw() = 0;
private:
};

