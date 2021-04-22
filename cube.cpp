#include "HIO_include.h"

float cube::points[][3] = { 
{ -0.5, -0.5, -0.5 },{ 0.5, -0.5, -0.5 },
{ 0.5, 0.5, -0.5 },{ -0.5, 0.5, -0.5 },
{ -0.5, -0.5, 0.5 },{ 0.5, -0.5, 0.5 },
{ 0.5, 0.5, 0.5 },{ -0.5, 0.5, 0.5 } };

float cube::text_coor[4][2] = {
	{ 0,0 },{0,1 },
	{1,1},{ 1,0 }};

int cube::face[][4] = { 
{ 0, 3, 2, 1 },{ 0, 1, 5, 4 },{ 1, 2, 6, 5 },
{ 4, 5, 6, 7 },{ 2, 3, 7, 6 },{ 0, 4, 7, 3 } };

void cube::cross(int i)
{
	float v1[3], v2[3];
	for (int t = 0; t < 3; ++t)
	{
		v1[t] = points[face[i][1]][t] - points[face[i][2]][t];
		v2[t] = points[face[i][1]][t] - points[face[i][0]][t];
	}

	n = vector_3f((v1[1] * v2[2] - v1[2] * v2[1]),
				  (v1[2] * v2[0] - v1[0] * v2[2]),
				  (v1[0] * v2[1] - v1[1] * v2[0]));
}

cube::cube()
{
}


cube::~cube()
{
}

void cube::draw()
{
	int    i;

	for (i = 0; i<6; i++) {

		//glColor3fv(colors[i]);  /* Set color */
		glBegin(GL_POLYGON);  /* Draw the face */
		cross(i);
		//printf("%.1f, %.1f, %.1f\n", n[0], n[1], n[2]);
		
		glNormal3f(n[0], n[1], n[2]);
		glTexCoord2fv(text_coor[0]);
		glVertex3fv(points[face[i][0]]);
		glTexCoord2fv(text_coor[1]);
		glVertex3fv(points[face[i][1]]);
		glTexCoord2fv(text_coor[2]);
		glVertex3fv(points[face[i][2]]);
		glTexCoord2fv(text_coor[3]);
		glVertex3fv(points[face[i][3]]);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}


