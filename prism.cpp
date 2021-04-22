#include "HIO_include.h"

GLUquadricObj *prism::cylind = NULL;

prism::prism()
{
	if (cylind == NULL) {
		cylind = gluNewQuadric();
		gluQuadricDrawStyle(cylind, GLU_FILL);
		gluQuadricNormals(cylind, GLU_SMOOTH);
	}
}


prism::~prism()
{
}

void prism::draw()
{
	gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
		1.0,             /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */
	glDisable(GL_TEXTURE_2D);
}
