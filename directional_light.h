#pragma once
#include "light.h"
class directional_light :
	public light
{
protected:
	void init();
	void turnOn();
	void turnOff();
public:
	directional_light();
	directional_light(vector_3f d, GLenum id);
	void draw();
	void draw_light();
	~directional_light();

	vector_3f dir;
};

