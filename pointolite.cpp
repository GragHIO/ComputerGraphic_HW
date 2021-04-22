#include "HIO_include.h"

void pointolite::init()
{
	//glPushMatrix();
	//glLoadIdentity();
	glLightfv(GL_LIGHT0 + light_id, GL_POSITION, vector_4f(position, 1));
	glLightf(GL_LIGHT0 + light_id, GL_SPOT_CUTOFF, 360);
	//glPopMatrix();
}

void pointolite::turnOn()
{
	//glPushMatrix();
	//glLoadIdentity();
	glLightfv(GL_LIGHT0 + light_id, GL_POSITION, vector_4f(position, 1));

	//glPopMatrix();
}

void pointolite::turnOff()
{
}

pointolite::pointolite():position(0)
{

}

pointolite::pointolite(vector_3f pos, GLenum id):position(pos), light(id)
{
}

void pointolite::draw()
{
	glDisable(GL_TEXTURE_2D);
}

void pointolite::draw_light()
{
	light::draw_light();
	bool light_main_state = glIsEnabled(GL_LIGHTING);
	bool is_been_cover = glIsEnabled(GL_CULL_FACE);

	if (is_been_cover)
		glDisable(GL_CULL_FACE);

	if (light_state)
	{
		glColor3f(diffuse[0], diffuse[1], diffuse[2]);
		glLightfv(GL_LIGHT0 + light_id, GL_POSITION, vector_4f(position, 1));
	}
	else
		glColor3f(0, 0, 0);

	glTranslatef(position[0], position[1], position[2]);
	if(light_state)
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


pointolite::~pointolite()
{
}
