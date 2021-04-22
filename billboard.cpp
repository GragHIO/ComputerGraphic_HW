#include "HIO_include.h"

vector_3f billboard::b = { 0,1,0 };

billboard::billboard()
{
	P = { 0,0,0 };
}

billboard::billboard(float ww, float hh) :w(ww), h(hh)
{
	P = { 0,0,0 };
}


billboard::~billboard()
{
}

void billboard::draw()
{
	vector_3f A = get_a();
	vector_3f v0 = (P - get_a()*(w / 2.0));
	vector_3f v1 = (P + get_a()*(w / 2.0));
	vector_3f v2 = v1 + b*h;
	vector_3f v3 = v0 + b*h;
	glBegin(GL_QUADS);
	/*glNormal3fv((vector_4f)c);
	glTexCoord2f(0, 0);
	glVertex3fv((vector_4f)v0);
	glTexCoord2f(1, 0);
	glVertex3fv((vector_4f)v1);
	glTexCoord2f(1, 1);
	glVertex3fv((vector_4f)(v1 + b*h));
	glTexCoord2f(0, 1);
	glVertex3fv((vector_4f)(v0 + b*h));	
	*/
	glTexCoord2f(0, 0);
	glVertex3f( - 5.0 * c.m_x, -1.0,  - 5.0 * c.m_z);
	glTexCoord2f(1, 0);
	glVertex3f( + 5.0  * c.m_x, -1.0,  + 5.0 * c.m_z);
	glTexCoord2f(1, 1);
	glVertex3f( + 5.0  * c.m_x, 9.0,  + 5.0  * c.m_z);
	glTexCoord2f(0, 1);
	glVertex3f(- 5.0* c.m_x, 9.0,  - 5.0 * c.m_z);

	
	glEnd();
	glDisable(GL_TEXTURE_2D);


	
}

vector_3f billboard::get_a()
{
	return vector_3f(c.m_z / sqrtf(c.m_z*c.m_z + c.m_x*c.m_x),
		             0,
		c.m_x / sqrtf(c.m_z*c.m_z + c.m_x*c.m_x));
}
