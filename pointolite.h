#pragma once
#include "light.h"
class pointolite :
	public light
{
protected:
	void init();
	void turnOn();
	void turnOff();
public:
	pointolite();
	pointolite(vector_3f pos, GLenum id);
	void draw();
	void draw_light();
	~pointolite();

	vector_3f position;
};

