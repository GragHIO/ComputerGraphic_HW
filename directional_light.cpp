#include "HIO_include.h"

void directional_light::init()
{
	glLightfv(get_light_id(), GL_POSITION, vector_4f(dir, 0));
}

void directional_light::turnOn()
{
	glLightfv(get_light_id(), GL_POSITION, vector_4f(dir,0));
}

void directional_light::turnOff()
{

}

directional_light::directional_light() :dir({0,1,0})
{

}

directional_light::directional_light(vector_3f d, GLenum id):dir(d), light(id)
{
	d > 0 ?	dir = ~dir : dir = { 0,1,0 };
}

void directional_light::draw()
{
	glDisable(GL_TEXTURE_2D);
}

void directional_light::draw_light()
{
	light::draw_light();
	bool light_main_state = glIsEnabled(GL_LIGHTING);
	bool is_been_cover = glIsEnabled(GL_CULL_FACE);

	glPushMatrix();
		float My_high_Near = MyZNear*tan(PI*MyFovy / 360);
		float My_width_Near = My_high_Near*MyAspect;
		float My_high_Far = MyZFar*tan(PI*MyFovy / 360);
		float My_width_Far = My_high_Far*MyAspect;
		float a[3] = { eye[0] - u[2][0] * MyZFar + u[1][0] * My_high_Far + u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar + u[1][1] * My_high_Far + u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar + u[1][2] * My_high_Far + u[0][2] * My_width_Far };
		float b[3] = { eye[0] - u[2][0] * MyZFar - u[1][0] * My_high_Far + u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar - u[1][1] * My_high_Far + u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar - u[1][2] * My_high_Far + u[0][2] * My_width_Far };
		float c[3] = { eye[0] - u[2][0] * MyZFar - u[1][0] * My_high_Far - u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar - u[1][1] * My_high_Far - u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar - u[1][2] * My_high_Far - u[0][2] * My_width_Far };
		float v1[3], v2[3];
		for (int i = 0; i < 3; ++i)
		{
			v1[i] = b[i] - c[i];
			v2[i] = b[i] - a[i];
		}
		vector_3f f = vector_3f((v1[1] * v2[2] - v1[2] * v2[1]),
								(v1[2] * v2[0] - v1[0] * v2[2]),
								(v1[0] * v2[1] - v1[1] * v2[0]));
		float k = -1 * (f[0] * a[0] + f[1] * a[1] + f[2] * a[2]);
		float t;
		if ((f[0] * dir[0] + f[1] * dir[1] + f[2] * dir[2]) == 0)t = 0;
		else t = -1 * (eye[0] * f[0] + eye[1] * f[1] + eye[2] * f[2] + k) / (f[0] * dir[0] + f[1] * dir[1] + f[2] * dir[2]);

		if (t <= 0)t = 0;

		glLoadIdentity();
		gluLookAt(eye[0], eye[1], eye[2],
			eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
			u[1][0], u[1][1], u[1][2]);
		//gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		if (is_been_cover)
			glDisable(GL_CULL_FACE);

		if (light_state)
		{
			glColor3f(diffuse[0], diffuse[1], diffuse[2]);
			glLightfv(get_light_id(), GL_POSITION, vector_4f(dir, 0));
		}
		else
			glColor3f(0, 0, 0);

		//glTranslatef(dir[0], dir[1], dir[2]);

		if (light_state)
			glMaterialfv(GL_FRONT, GL_EMISSION, diffuse);
		else
			glMaterialfv(GL_FRONT, GL_EMISSION, vector_4f(0));
	
		if (t > 0) {
			glPushMatrix();
				/*glBegin(GL_LINES);
				glVertex3f(eye[0], eye[1], eye[2]);
				glVertex3f(eye[0] + dir[0] * t, eye[1] + dir[1] * t, eye[2] + dir[2] * t);
				glEnd();*/
				glTranslatef(eye[0] + dir[0] * t, eye[1] + dir[1] * t, eye[2] + dir[2] * t);
				glutSolidSphere(1, 12, 12);
			glPopMatrix();
		}


		glMaterialfv(GL_FRONT, GL_EMISSION, vector_4f(0));

		if (light_main_state && global_light)
			glEnable(GL_LIGHTING);
		if (is_been_cover)
			glEnable(GL_CULL_FACE);

	glPopMatrix();
}

directional_light::~directional_light()
{
}
