#include "HIO_include.h"

GLUquadricObj  *ball::sphere = NULL;

ball::ball()
{
	if (sphere == NULL) {
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricNormals(sphere, GLU_SMOOTH);
	}
}

ball::~ball()
{
}

void ball::draw()
{
	gluSphere(sphere, 1.0, 12, 12);
	glDisable(GL_TEXTURE_2D);
}
