#include "HIO_include.h"



void light::init()
{
}

GLenum light::get_light_id() {
	switch (light_id)
	{
	case 1:
		return GL_LIGHT1;
		break;
	case 2:
		return GL_LIGHT2;
		break;
	case 3:
		return GL_LIGHT3;
		break;
	case 4:
		return GL_LIGHT4;
		break;
	case 5:
		return GL_LIGHT5;
		break;
	case 6:
		return GL_LIGHT6;
		break;
	case 7:
		return GL_LIGHT7;
		break;
	case 0:
	default:
		return GL_LIGHT0;
		break;
	}
}

light::light():light_id(0)
{
	glEnable(GL_LIGHTING);
	glEnable(get_light_id());
	init();
	glLightfv(get_light_id(), GL_DIFFUSE, diffuse);
	glLightfv(get_light_id(), GL_SPECULAR, specular);
	glDisable(get_light_id());
	glDisable(GL_LIGHTING);
}

light::light(int id):light_id(id)
{
	glEnable(GL_LIGHTING);
	glEnable(get_light_id());
	init();
	glLightfv(get_light_id(), GL_DIFFUSE, diffuse);
	glLightfv(get_light_id(), GL_SPECULAR, specular);
	glDisable(get_light_id());
	glDisable(GL_LIGHTING);
}


light::~light()
{
	close();
}

void light::open()
{
	
	//glEnable(GL_LIGHT0 + light_id);
	light_state = true;
	turnOn();
}

void light::close()
{
	light_state = false;
	turnOff();
	
	//glDisable(GL_LIGHT0 + light_id);
}

void light::draw_light()
{
	if (!global_light) return;
	
	if (light_state) {
		glEnable(get_light_id());
		glLightfv(get_light_id(), GL_DIFFUSE, diffuse);
		glLightfv(get_light_id(), GL_SPECULAR, specular);
	}
	else
		glDisable(get_light_id());

}




