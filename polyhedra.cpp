#include "HIO_include.h"

polyhedra::polyhedra()
{
	uds = NULL;
}

polyhedra::polyhedra(char * name)
{
	uds = obj_create(name);	
}

polyhedra::~polyhedra()
{
}

void polyhedra::draw()
{
	glEnable(GL_TEXTURE_2D);
	draw_polyhedron(uds);
	glDisable(GL_TEXTURE_2D);
}


