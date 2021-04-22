#pragma once
#include "light.h"
class Spot_light :
	public light
{
protected:
	void init();
	void turnOn();
	void turnOff();
public:
	Spot_light();
	Spot_light(vector_3f pos, vector_3f d, float c, float e, GLenum id);
	void draw();
	void draw_light();
	~Spot_light();

	vector_3f position;
	vector_3f dir;
	float cutoff;
	float exponent;
};

