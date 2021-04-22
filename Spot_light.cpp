#include "HIO_include.h"


void Spot_light::init()
{
	glLightfv(get_light_id(), GL_POSITION, vector_4f(position, 1));
	glLightfv(get_light_id(), GL_SPOT_DIRECTION, vector_4f(dir, 0));
	glLightf(get_light_id(), GL_SPOT_CUTOFF, cutoff);
	glLightf(get_light_id(), GL_CONSTANT_ATTENUATION, 1);
	glLightf(get_light_id(), GL_QUADRATIC_ATTENUATION, 0);
	glLightf(get_light_id(), GL_LINEAR_ATTENUATION, 0);
	glLightf(get_light_id(), GL_SPOT_EXPONENT, exponent);
}

void Spot_light::turnOn()
{

	glLightfv(get_light_id(), GL_POSITION, vector_4f(position, 1));
	glLightfv(get_light_id(), GL_SPOT_DIRECTION, vector_4f(dir, 0));
	glLightf(get_light_id(), GL_SPOT_CUTOFF, cutoff);
	glLightf(get_light_id(), GL_CONSTANT_ATTENUATION, 1);
	glLightf(get_light_id(), GL_QUADRATIC_ATTENUATION, 0);
	glLightf(get_light_id(), GL_LINEAR_ATTENUATION, 0);
	glLightf(get_light_id(), GL_SPOT_EXPONENT, exponent);
}

void Spot_light::turnOff()
{
}

Spot_light::Spot_light() :position(0), dir({0,1,0}), cutoff(180), exponent(0)
{
}

Spot_light::Spot_light(vector_3f pos, vector_3f d, float c, float e, GLenum id) :position(pos), dir(d), cutoff(c), exponent(e), light(id)
{
	d > 0 ? dir = ~dir : dir = { 0,1,0 };
}

void Spot_light::draw()
{
	glDisable(GL_TEXTURE_2D);
}

void Spot_light::draw_light()
{
	light::draw_light();
	bool light_main_state = glIsEnabled(GL_LIGHTING);
	bool is_been_cover = glIsEnabled(GL_CULL_FACE);

	if (is_been_cover)
		glDisable(GL_CULL_FACE);

	if (light_state)
	{
		glColor3f(diffuse[0], diffuse[1], diffuse[2]);
		glLightfv(get_light_id(), GL_POSITION, vector_4f(position, 1));
		glLightfv(get_light_id(), GL_SPOT_DIRECTION, vector_4f(dir, 0));
		glLightf(get_light_id(), GL_SPOT_CUTOFF, cutoff);
		glLightf(get_light_id(), GL_CONSTANT_ATTENUATION, 1);
		glLightf(get_light_id(), GL_QUADRATIC_ATTENUATION, 0);
		glLightf(get_light_id(), GL_LINEAR_ATTENUATION, 0);
		glLightf(get_light_id(), GL_SPOT_EXPONENT, exponent);
	}
	else
		glColor3f(0, 0, 0);

	glTranslatef(position[0], position[1], position[2]);
	if (light_state)
		glMaterialfv(GL_FRONT, GL_EMISSION, diffuse);
	else
		glMaterialfv(GL_FRONT, GL_EMISSION, vector_4f(0));
	glutSolidSphere(1, 12, 12);
	glMaterialfv(GL_FRONT, GL_EMISSION, vector_4f(0));

	if (light_main_state && global_light)
		glEnable(GL_LIGHTING);
	if (is_been_cover)
		glEnable(GL_CULL_FACE);
}


Spot_light::~Spot_light()
{
}
