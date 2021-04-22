#include "HIO_include.h"

trans::trans()
{
	m_angle = { 0, 0, 0 };
	m_scale = { 1, 1, 1 };
	m_shift = { 1, 1, 1 };
}

trans::trans(vector_3f angle, vector_3f scale, vector_3f shift)
{
	m_angle = angle;
	m_scale = scale;
	m_shift = shift;
}


trans::~trans()
{
}

void trans::draw()
{	
	glTranslatef(m_shift.m_x, m_shift.m_y, m_shift.m_z);
	glRotatef(m_angle.m_x, 1, 0, 0);
	glRotatef(m_angle.m_y, 0, 1, 0);
	glRotatef(m_angle.m_z, 0, 0, 1);
	glScalef(m_scale.m_x, m_scale.m_y, m_scale.m_z);	

}


void trans::draw_light()
{
	glTranslatef(m_shift.m_x, m_shift.m_y, m_shift.m_z);
	glRotatef(m_angle.m_x, 1, 0, 0);
	glRotatef(m_angle.m_y, 0, 1, 0);
	glRotatef(m_angle.m_z, 0, 0, 1);
	glScalef(m_scale.m_x, m_scale.m_y, m_scale.m_z);
}
