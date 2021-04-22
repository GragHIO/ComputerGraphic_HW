
#include "HIO_include.h"

#include <typeinfo>
#include <iostream>

bool draw_boundingBox = false;
bool control_robot_mode = true;
GLUquadricObj  *sphere = NULL, *cylind = NULL, *disk;
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 0.0, 0.0, 30.0 }, Focus[3] = { 0.0, 0.0, 0.0 },
Vup[3] = { 0.0, 1.0, 0.0 };

float   cv, sv; 
enum style_t style = Free_axis;
int top_m, index_angle = 0;
float spot_light_angle = 0, robot_elevation = -90, sun_elevation = 0;
pointolite *test_light = new pointolite({0,15,0}, 0);
directional_light *test_light1 = new directional_light({ 0,0,1 },1);
Spot_light *test_light2 = new Spot_light({0.0f, 0.0f,0.0f}, { 0.0f, -1.0f, 0.0f }, 20, 1, 2);
vector_4f fan_material[4] = { { 0.5f, 2.0f, 0.7f, 5.0f },{ 0.2f, 0.9f, 0.8f, 32.0f }, { 0.0f, 0.1f, 0.6f, 128.0f },{ 0.2f, 0.9f, 0.8f, 9487.0f } };
vector_3f pointolite_color = { 1.0f,0.0f,0.0f }, pointolite_index = {0,0,0};
group fan_group = group("fan_color", { 1, 1, 1 }, fan_material[0]);
int fan_material_index = 0;
GLuint waterfall_texture = GL_ZERO;
GLuint keyFrame[24];
fog sence_fog;
int fog_i = 0;

void top_menu_func(int value)
{
	style = (style_t)value;
}
void main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));

	/*-----Initialize the GLUT environment-------*/
	glutInit(&argc, argv);

	/*-----Depth buffer is used, be careful !!!----*/
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Homework5");
	
	myinit();      /*---Initialize other state varibales----*/

				   /*----Associate callback func's whith events------*/
	glutDisplayFunc(display);
	glutIdleFunc(idle_func);    /* Add idle event */
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutReshapeWindow(width, height);
	top_m = glutCreateMenu(top_menu_func);
	glutAddMenuEntry("X_axis", X_axis);
	glutAddMenuEntry("Y_axis", Y_axis);
	glutAddMenuEntry("Z_axis", Z_axis);
	glutAddMenuEntry("Free_axis", Free_axis);
	glutAddMenuEntry("Three_axis", Three_axis);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}

void draw_axes()
{

	/*----Draw a white sphere to represent the original-----*/
	glMaterialfv(GL_FRONT, GL_AMBIENT, vector_4f(vector_3f(0.9, 0.9, 0.9) *0.3, 1));
	glMaterialfv(GL_FRONT, GL_SPECULAR, vector_4f(0.9, 0.9, 0.9, 1));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, vector_4f(vector_3f(0.9, 0.9, 0.9) *0.8, 1));
	glMaterialf(GL_FRONT, GL_SHININESS, 32);
	glColor3f(0.9, 0.9, 0.9);

	gluSphere(sphere, 2.0,   /* radius=2.0 */
		12,            /* composing of 12 slices*/
		12);           /* composing of 8 stacks */

					   /*----Draw three axes in colors, yellow, meginta, and cyan--*/
					   /* Draw Z axis  */
	glMaterialfv(GL_FRONT, GL_AMBIENT, vector_4f(vector_3f(0.0, 0.95, 0.95) *0.3, 1));
	glMaterialfv(GL_FRONT, GL_SPECULAR, vector_4f(0.0, 0.95, 0.95, 1));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, vector_4f(vector_3f(0.0, 0.95, 0.95) *0.8, 1));
	glMaterialf(GL_FRONT, GL_SHININESS, 32);
	glColor3f(0.0, 0.95, 0.95);
	gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
		10.0,              /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */

						  /* Draw Y axis */
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
	glMaterialfv(GL_FRONT, GL_AMBIENT, vector_4f(vector_3f(0.95, 0.0, 0.95) *0.3, 1));
	glMaterialfv(GL_FRONT, GL_SPECULAR, vector_4f(0.95, 0.0, 0.95, 1));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, vector_4f(vector_3f(0.95, 0.0, 0.95) *0.8, 1));
	glMaterialf(GL_FRONT, GL_SHININESS, 32);
	glColor3f(0.95, 0.0, 0.95);
	gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
		10.0,             /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */
	glPopMatrix();

	/* Draw X axis */
	glMaterialfv(GL_FRONT, GL_AMBIENT, vector_4f(vector_3f(0.95, 0.95, 0.0) *0.3, 1));
	glMaterialfv(GL_FRONT, GL_SPECULAR, vector_4f(0.95, 0.95, 0.0, 1));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, vector_4f(vector_3f(0.95, 0.95, 0.0) *0.8, 1));
	glMaterialf(GL_FRONT, GL_SHININESS, 32);
	glColor3f(0.95, 0.95, 0.0);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
	gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
		10.0,             /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */
	glPopMatrix();
	/*-- Restore the original modelview matrix --*/
	//glPopMatrix();
}

void display_light_scene()
{
	
	glPushMatrix();
		glTranslatef(-25, 0, -25);
		glPushMatrix();
			dfs_draw_light(scene_graph.root);
		glPopMatrix();

	glPopMatrix();
}

void display_scene()
{
	glTranslatef(-25,0,-25);
	glPushMatrix();	
	draw_floor();

	glPushMatrix();
		draw_waterfall();
	glPopMatrix();
	draw_sae();
	draw_axes();
	glPushMatrix();
	dfs_draw(scene_graph.root);
	glPopMatrix();

	if (draw_boundingBox) {
		glPushMatrix();
		for (auto node_ptr = entity_list->first; node_ptr; node_ptr = node_ptr->next)
		{
			entity* data = (entity *)(node_ptr->data);
			AABB to_draw = data->bounding_box;
			trans * scene = (trans *)(scene_graph.root->son->first->data->data);
			glPushMatrix();
			glTranslatef(scene->m_shift.m_x, scene->m_shift.m_y, scene->m_shift.m_z);
			glTranslatef(to_draw.c.m_x, to_draw.c.m_y, to_draw.c.m_z);
			glScalef(to_draw.r.m_x * 2, to_draw.r.m_y * 2, to_draw.r.m_z * 2);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor3f(1, 0, 0);
			cube_node->data->draw();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();

}

void draw_view()
{
	int    i;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		float My_high_Near = MyZNear*tan(PI*MyFovy / 360);
		float My_width_Near = My_high_Near*MyAspect;
		float My_high_Far = MyZFar*tan(PI*MyFovy / 360);
		float My_width_Far = My_high_Far*MyAspect;

		/*----Draw Eye position-----*/
		glPushMatrix();
			glTranslatef(eye[0], eye[1], eye[2]);
			glColor3f(0.0, 1.0, 0.0);
			glutWireSphere(1.0, 10, 10);
		glPopMatrix();

		glColor3f(0.15, 0.30, 0.50);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		/*Draw Near Volumn*/
		glBegin(GL_LINES);
		glVertex3f(eye[0] - u[2][0] * MyZNear + u[1][0] * My_high_Near + u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear + u[1][1] * My_high_Near + u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear + u[1][2] * My_high_Near + u[0][2] * My_width_Near);
		glVertex3f(eye[0] - u[2][0] * MyZNear - u[1][0] * My_high_Near + u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear - u[1][1] * My_high_Near + u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear - u[1][2] * My_high_Near + u[0][2] * My_width_Near);
		glEnd();
		glBegin(GL_LINES);

		glVertex3f(eye[0] - u[2][0] * MyZNear - u[1][0] * My_high_Near + u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear - u[1][1] * My_high_Near + u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear - u[1][2] * My_high_Near + u[0][2] * My_width_Near);
		glVertex3f(eye[0] - u[2][0] * MyZNear - u[1][0] * My_high_Near - u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear - u[1][1] * My_high_Near - u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear - u[1][2] * My_high_Near - u[0][2] * My_width_Near);
		glEnd();
		glBegin(GL_LINES);

		glVertex3f(eye[0] - u[2][0] * MyZNear - u[1][0] * My_high_Near - u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear - u[1][1] * My_high_Near - u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear - u[1][2] * My_high_Near - u[0][2] * My_width_Near);
		glVertex3f(eye[0] - u[2][0] * MyZNear + u[1][0] * My_high_Near - u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear + u[1][1] * My_high_Near - u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear + u[1][2] * My_high_Near - u[0][2] * My_width_Near);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(eye[0] - u[2][0] * MyZNear + u[1][0] * My_high_Near - u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear + u[1][1] * My_high_Near - u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear + u[1][2] * My_high_Near - u[0][2] * My_width_Near);
		glVertex3f(eye[0] - u[2][0] * MyZNear + u[1][0] * My_high_Near + u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear + u[1][1] * My_high_Near + u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear + u[1][2] * My_high_Near + u[0][2] * My_width_Near);
		glEnd();
		/*Draw Far Volumn*/
		glBegin(GL_LINES);
		glVertex3f(eye[0] - u[2][0] * MyZFar + u[1][0] * My_high_Far + u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar + u[1][1] * My_high_Far + u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar + u[1][2] * My_high_Far + u[0][2] * My_width_Far);
		glVertex3f(eye[0] - u[2][0] * MyZFar - u[1][0] * My_high_Far + u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar - u[1][1] * My_high_Far + u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar - u[1][2] * My_high_Far + u[0][2] * My_width_Far);
		glEnd();
		glBegin(GL_LINES);

		glVertex3f(eye[0] - u[2][0] * MyZFar - u[1][0] * My_high_Far + u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar - u[1][1] * My_high_Far + u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar - u[1][2] * My_high_Far + u[0][2] * My_width_Far);
		glVertex3f(eye[0] - u[2][0] * MyZFar - u[1][0] * My_high_Far - u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar - u[1][1] * My_high_Far - u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar - u[1][2] * My_high_Far - u[0][2] * My_width_Far);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(eye[0] - u[2][0] * MyZFar - u[1][0] * My_high_Far - u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar - u[1][1] * My_high_Far - u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar - u[1][2] * My_high_Far - u[0][2] * My_width_Far);
		glVertex3f(eye[0] - u[2][0] * MyZFar + u[1][0] * My_high_Far - u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar + u[1][1] * My_high_Far - u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar + u[1][2] * My_high_Far - u[0][2] * My_width_Far);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(eye[0] - u[2][0] * MyZFar + u[1][0] * My_high_Far - u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar + u[1][1] * My_high_Far - u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar + u[1][2] * My_high_Far - u[0][2] * My_width_Far);
		glVertex3f(eye[0] - u[2][0] * MyZFar + u[1][0] * My_high_Far + u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar + u[1][1] * My_high_Far + u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar + u[1][2] * My_high_Far + u[0][2] * My_width_Far);
		glEnd();
		/*Drae Adges*/
		glBegin(GL_LINES);
		glVertex3f(eye[0] - u[2][0] * MyZNear + u[1][0] * My_high_Near + u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear + u[1][1] * My_high_Near + u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear + u[1][2] * My_high_Near + u[0][2] * My_width_Near);
		glVertex3f(eye[0] - u[2][0] * MyZFar + u[1][0] * My_high_Far + u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar + u[1][1] * My_high_Far + u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar + u[1][2] * My_high_Far + u[0][2] * My_width_Far);
		glEnd();
		glBegin(GL_LINES);

		glVertex3f(eye[0] - u[2][0] * MyZNear - u[1][0] * My_high_Near + u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear - u[1][1] * My_high_Near + u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear - u[1][2] * My_high_Near + u[0][2] * My_width_Near);
		glVertex3f(eye[0] - u[2][0] * MyZFar - u[1][0] * My_high_Far + u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar - u[1][1] * My_high_Far + u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar - u[1][2] * My_high_Far + u[0][2] * My_width_Far);
		glEnd();
		glBegin(GL_LINES);

		glVertex3f(eye[0] - u[2][0] * MyZNear - u[1][0] * My_high_Near - u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear - u[1][1] * My_high_Near - u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear - u[1][2] * My_high_Near - u[0][2] * My_width_Near);
		glVertex3f(eye[0] - u[2][0] * MyZFar - u[1][0] * My_high_Far - u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar - u[1][1] * My_high_Far - u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar - u[1][2] * My_high_Far - u[0][2] * My_width_Far);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(eye[0] - u[2][0] * MyZNear + u[1][0] * My_high_Near - u[0][0] * My_width_Near,
			eye[1] - u[2][1] * MyZNear + u[1][1] * My_high_Near - u[0][1] * My_width_Near,
			eye[2] - u[2][2] * MyZNear + u[1][2] * My_high_Near - u[0][2] * My_width_Near);
		glVertex3f(eye[0] - u[2][0] * MyZFar + u[1][0] * My_high_Far - u[0][0] * My_width_Far,
			eye[1] - u[2][1] * MyZFar + u[1][1] * My_high_Far - u[0][1] * My_width_Far,
			eye[2] - u[2][2] * MyZFar + u[1][2] * My_high_Far - u[0][2] * My_width_Far);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
	/*----Draw eye coord. axes -----*/
	/*glColor3f(1.0, 1.0, 0.0);           // Draw Xe
	glBegin(GL_LINES);
	glVertex3f(eye[0], eye[1], eye[2]);
	glVertex3f(eye[0] + 20.0*u[0][0], eye[1] + 20.0*u[0][1], eye[2] + 20.0*u[0][2]);
	glEnd();

	glColor3f(1.0, 0.0, 1.0);          // Draw Ye
	glBegin(GL_LINES);
	glVertex3f(eye[0], eye[1], eye[2]);
	glVertex3f(eye[0] + 20.0*u[1][0], eye[1] + 20.0*u[1][1], eye[2] + 20.0*u[1][2]);
	glEnd();

	glColor3f(0.0, 1.0, 1.0);          // Draw Ze
	glBegin(GL_LINES);
	glVertex3f(eye[0], eye[1], eye[2]);
	glVertex3f(eye[0] + 20.0*u[2][0], eye[1] + 20.0*u[2][1], eye[2] + 20.0*u[2][2]);
	glEnd();*/
}

void make_view(style_t x)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	switch (x) {
	case Free_axis:
		gluLookAt(eye[0], eye[1], eye[2],
			eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
			u[1][0], u[1][1], u[1][2]);
		break;

	case X_axis:       /* X direction parallel viewing */
		gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;
	case Y_axis:       /* Y direction parallel viewing */
		gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
		break;
	case Z_axis:
		gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;
	}
	if (flower != NULL)
	{
		flower->c = { u[0][0],u[0][1],u[0][2] };
		flower->c = ~flower->c;
	}
	/*
	glPushMatrix();	
		glLightfv(GL_LIGHT0, GL_POSITION, vector_4f(10,0,0,1));
	glPopMatrix();
	*/
	display_light_scene();
}

void make_projection(style_t x) 
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (x == Free_axis) {
		gluPerspective(MyFovy, MyAspect, MyZNear, MyZFar);
	}
	else {
		if (width>height)
			glOrtho(-40.0, 40.0, -40.0*(float)height / (float)width,
				40.0*(float)height / (float)width,
				-0.0, 100.0);
		else
			glOrtho(-40.0*(float)width / (float)height,
				40.0*(float)width / (float)height, -40.0, 40.0,
				-0.0, 100.0);
	}
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*----Define the current eye position and the eye-coordinate system---*/
	if (sence_fog.type)
	{
		glEnable(GL_FOG);
		glClearColor(sence_fog.color.m_x, sence_fog.color.m_y, sence_fog.color.m_z, sence_fog.color.m_w);

		switch (sence_fog.type)
		{
			case 1:
				glFogi(GL_FOG_MODE, GL_EXP);
				break;
			case 2:
				glFogi(GL_FOG_MODE, GL_EXP2);
				break;
			case 3:
				glFogi(GL_FOG_MODE, GL_LINEAR);
				glFogf(GL_FOG_START, sence_fog.start);
				glFogf(GL_FOG_END, sence_fog.end);
				break;

			default:				
				break;
		}
		glFogfv(GL_FOG_COLOR, sence_fog.color);
		glFogf(GL_FOG_DENSITY, sence_fog.density);
	}
	else {
		glDisable(GL_FOG);
		glClearColor(0,0,0,0);
	}


	switch (style) {
	case Three_axis:

		glViewport(0, height / 2, width / 2, height / 2);
		make_view(X_axis);
		
		make_projection(X_axis);	
		draw_view();
		display_scene();
		

		glViewport(width / 2, height / 2, width / 2, height / 2);
		make_view(Y_axis);
		
		make_projection(Y_axis);
		draw_view();
		display_scene();
		

		glViewport(0, 0, width / 2, height / 2);
		make_view(Z_axis);
		
		make_projection(Z_axis);
		draw_view();
		display_scene();
		
		glViewport(width / 2, 0, width / 2, height / 2);
		make_view(Free_axis);
		make_projection(Free_axis);
		display_scene();

		break;

	case X_axis:
	case Y_axis:
	case Z_axis:
	case Free_axis:
		glViewport(0, 0, width, height);
		make_view(style);
		make_projection(style);
		display_scene();
		break;
	}

	

	/*-------Swap the back buffer to the front --------*/
	glutSwapBuffers();
	glFlush(); /*--Display the results----*/
}

void  myinit()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)printf("Error: %s\n", glewGetErrorString(err));
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		cout << "Ready for GLSL" << endl;
	else {
		cout << "No GLSL support" << endl;
		exit(1);
	}
	/*BLEND*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,  //source factor
		GL_ONE_MINUS_SRC_ALPHA); //dst factor.


	/*ALPHA_TEST*/
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);



	glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
										   /*Clear the Depth & Color Buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	//glViewport(0, 0, 50, 50);
	if (sphere == NULL) {
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricNormals(sphere, GLU_SMOOTH);
	}
	if (cylind == NULL) {
		cylind = gluNewQuadric();
		gluQuadricDrawStyle(cylind, GLU_FILL);
		gluQuadricNormals(cylind, GLU_SMOOTH);
	}
	if (disk == NULL) {
		disk = gluNewQuadric();
		gluQuadricDrawStyle(disk, GLU_FILL);
		gluQuadricNormals(disk, GLU_SMOOTH);
	}
	/*-----Set a parallel projection mode-----*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 20.0);

	glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */

	glFlush();/*Enforce window system display the results*/

	 /*---- Compute cos(5.0) and sin(5.0) ----*/
	cv = cos(5.0*PI / 180.0);
	sv = sin(5.0*PI / 180.0);

	/*---- Copy eye position ---*/
	eye[0] = Eye[0];
	eye[1] = Eye[1];
	eye[2] = Eye[2];

	init_scene();
	init_robot();
	init_entity();

	glDisable(GL_LIGHTING);
	global_light = false;

	waterfall_texture = LoadTextureBMP("waterfall.bmp", true);

	for (int i = 0; i < 24; ++i)
	{
		keyFrame[i] = LoadTextureBMP((std::string("t1/t") + std::to_string(i) + ".bmp").c_str(), true);
	}
}

void reshape(int w, int h)
{
	width = w;
	height = h;
	MyAspect = ((double)w / (double)h);
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(MyFovy, MyAspect, MyZNear, MyZFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int xx, int yy)
{
	int    i;
	float  x[3], y[3], z[3];
	GLint old_mat_mode = 0;
	float m[4][4] = {0};
	if (key == '=')
	{
		control_robot_mode = !control_robot_mode;
	}
	if (control_robot_mode)
	{
		if (key == '\'')
		{
			draw_boundingBox = !draw_boundingBox;
		}
		if (key == '`') //ｦ^ｨ・・I
		{
			robot_entity->robot_coord->m_shift = { 0,50,0 };
			robot_entity->move_x = { 0,50,0 };
		}
		if (key == 'w') //ｩｹｫeｨｫ
		{
			robot_entity->is_walking = true;
		}
		if (key == 'W') //ｩｹｫeｶ]
		{
			robot_entity->is_running = true;
		}
		if (key == 'q' || key == 'Q') //･ｪﾂ・
		{
			glPushMatrix();
			glGetIntegerv(GL_MATRIX_MODE, &old_mat_mode);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glRotatef(30, 0, 1, 0);
			glTranslatef(robot_entity->facing.m_x, robot_entity->facing.m_y, robot_entity->facing.m_z);
			glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m);
			glPopMatrix();
			glMatrixMode(old_mat_mode);
			glPopMatrix();
			robot_entity->facing = { m[3][0], m[3][1], m[3][2] };
			robot_entity->robot_coord->m_angle += {0, 30, 0};
		}
		if (key == 'e' || key == 'E') //･kﾂ・
		{
			glPushMatrix();
			glGetIntegerv(GL_MATRIX_MODE, &old_mat_mode);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glRotatef(-30, 0, 1, 0);
			glTranslatef(robot_entity->facing.m_x, robot_entity->facing.m_y, robot_entity->facing.m_z);
			glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m);
			glPopMatrix();
			glMatrixMode(old_mat_mode);
			glPopMatrix();
			robot_entity->facing = { m[3][0], m[3][1], m[3][2] };
			robot_entity->robot_coord->m_angle += {0, -30, 0};
		}
		if (key == ' ') //ｸ・
		{
			robot_entity->is_jumping = true;
		}
		if (key == '\\') //ｲ｣･ﾍ､雜・
		{
			add_udo_cube();
		}
		if (key == 'a' || key == 'A') //･ｪｪﾓｩｹｫ・
		{
			if (robot_entity->l_shoulder->m_angle.m_x < 80)
				robot_entity->l_shoulder->m_angle.m_x += 10;
		}
		if (key == 's' || key == 'S') //･ｪｪﾓｩｹｫe
		{
			if (robot_entity->l_shoulder->m_angle.m_x > -190)
				robot_entity->l_shoulder->m_angle.m_x -= 10;
		}
		if (key == 'd' || key == 'D') //･ｪｪﾓｩｹ･~
		{
			if (robot_entity->l_shoulder->m_angle.m_z < 90)
				robot_entity->l_shoulder->m_angle.m_z += 10;
		}
		if (key == 'f' || key == 'F') //･ｪｪﾓｩｹ､ｺ
		{
			if (robot_entity->l_shoulder->m_angle.m_z > 0)
				robot_entity->l_shoulder->m_angle.m_z -= 10;
		}
		if (key == 'z' || key == 'Z') //･kｪﾓｩｹｫ・
		{
			if (robot_entity->r_shoulder->m_angle.m_x < 80)
				robot_entity->r_shoulder->m_angle.m_x += 10;
		}
		if (key == 'x' || key == 'X') //･kｪﾓｩｹｫe
		{
			if (robot_entity->r_shoulder->m_angle.m_x > -190)
				robot_entity->r_shoulder->m_angle.m_x -= 10;
		}
		if (key == 'c' || key == 'C') //･kｪﾓｩｹ･~
		{
			if (robot_entity->r_shoulder->m_angle.m_z > -90)
				robot_entity->r_shoulder->m_angle.m_z -= 10;
		}
		if (key == 'v' || key == 'V') //･kｪﾓｩｹ､ｺ
		{
			if (robot_entity->r_shoulder->m_angle.m_z < 0)
				robot_entity->r_shoulder->m_angle.m_z += 10;
		}
		if (key == 'g' || key == 'G') //･ｪｨyｩｹｫe
		{
			if (robot_entity->l_elbow->m_angle.m_x > -140)
				robot_entity->l_elbow->m_angle.m_x -= 10;
		}
		if (key == 'h' || key == 'H') //･ｪｨyｩｹｫ・
		{
			if (robot_entity->l_elbow->m_angle.m_x < 0)
				robot_entity->l_elbow->m_angle.m_x += 10;
		}
		if (key == 'b' || key == 'B') //･kｨyｩｹｫe
		{
			if (robot_entity->r_elbow->m_angle.m_x > -140)
				robot_entity->r_elbow->m_angle.m_x -= 10;
		}
		if (key == 'n' || key == 'N') //･kｨyｩｹｫ・
		{
			if (robot_entity->r_elbow->m_angle.m_x < 0)
				robot_entity->r_elbow->m_angle.m_x += 10;
		}
		if (key == 'o' || key == 'O')
		{
			sence_fog.type = (sence_fog.type+1) % 4;
		}
		if (key == 'l' || key == 'L')
		{
			sence_fog.color = sence_fog.color_arr[fog_i = (fog_i+1) % 3];
		}
		if (key == ',')
		{
			if (sence_fog.start < sence_fog.end - 1)
				sence_fog.start++;
		}
		if (key == '<')
		{
			if (sence_fog.start > 1)
				sence_fog.start--;
		}
		if (key == '.')
		{
			if (sence_fog.end < 50)
				sence_fog.end++;
		}
		if (key == '>')
		{
			if (sence_fog.end > sence_fog.start)
				sence_fog.end--;
		}
		if (key == '/')
		{
			if (sence_fog.density < 1)
				sence_fog.density += 0.01;
		}
		if (key == '?')
		{
			if (sence_fog.density > 0.01)
				sence_fog.density -= 0.01;
		}
	}
	else
	{
		/*------transform the EYE coordinate system ------*/
	
		if (key == 'c')  /* move down */
		{
			eyeDy += 0.5;      
			for (i = 0; i<3; i++) eye[i] -= 0.5*u[1][i];
		}
		else if (key == ' ')  /* move up */
		{
			eyeDy += -0.5;     
			for (i = 0; i<3; i++) eye[i] += 0.5*u[1][i];
		}
		else if (key == 'd')  /* move right */
		{
			eyeDx += -0.5;      
			for (i = 0; i<3; i++) eye[i] += 0.5*u[0][i];
		}
		else if (key == 'a') /* move left */
		{
			eyeDx += 0.5;       
			for (i = 0; i<3; i++) eye[i] -= 0.5*u[0][i];
		}
		else if (key == 'w') /* zoom in */
		{
			eyeDz += 0.5;        
			for (i = 0; i<3; i++) eye[i] -= 0.5*u[2][i];
		}
		else if (key == 's')  /* zoom out */
		{
			eyeDz += -0.5;      
			for (i = 0; i<3; i++) eye[i] += 0.5*u[2][i];
		}
		else if (key == 'p') /* pitching */
		{             
			eyeAngx += 5.0;
			if (eyeAngx > 360.0) eyeAngx -= 360.0;
			y[0] = u[1][0] * cv - u[2][0] * sv;
			y[1] = u[1][1] * cv - u[2][1] * sv;
			y[2] = u[1][2] * cv - u[2][2] * sv;

			z[0] = u[2][0] * cv + u[1][0] * sv;
			z[1] = u[2][1] * cv + u[1][1] * sv;
			z[2] = u[2][2] * cv + u[1][2] * sv;

			for (i = 0; i<3; i++) {
				u[1][i] = y[i];
				u[2][i] = z[i];
			}
		}
		else if (key == 'P') 
		{
			eyeAngx += -5.0;
			if (eyeAngx<0.0) eyeAngx += 360.0;
			y[0] = u[1][0] * cv + u[2][0] * sv;
			y[1] = u[1][1] * cv + u[2][1] * sv;
			y[2] = u[1][2] * cv + u[2][2] * sv;

			z[0] = u[2][0] * cv - u[1][0] * sv;
			z[1] = u[2][1] * cv - u[1][1] * sv;
			z[2] = u[2][2] * cv - u[1][2] * sv;

			for (i = 0; i<3; i++) {
				u[1][i] = y[i];
				u[2][i] = z[i];
			}
		}
		else if (key == 'h') /* heading */
		{           
			eyeAngy += 5.0;
			if (eyeAngy>360.0) eyeAngy -= 360.0;
			for (i = 0; i<3; i++) {
				x[i] = cv*u[0][i] - sv*u[2][i];
				z[i] = sv*u[0][i] + cv*u[2][i];
			}
			for (i = 0; i<3; i++) {
				u[0][i] = x[i];
				u[2][i] = z[i];
			}
		}
		else if (key == 'H') 
		{
			eyeAngy += -5.0;
			if (eyeAngy<0.0) eyeAngy += 360.0;
			for (i = 0; i<3; i++) {
				x[i] = cv*u[0][i] + sv*u[2][i];
				z[i] = -sv*u[0][i] + cv*u[2][i];
			}
			for (i = 0; i<3; i++) {
				u[0][i] = x[i];
				u[2][i] = z[i];
			}
		}
		else if (key == 'r')  /* rolling */
		{           
			eyeAngz += 5.0;
			if (eyeAngz>360.0) eyeAngz -= 360.0;
			for (i = 0; i<3; i++) {
				x[i] = cv*u[0][i] - sv*u[1][i];
				y[i] = sv*u[0][i] + cv*u[1][i];
			}
			for (i = 0; i<3; i++) {
				u[0][i] = x[i];
				u[1][i] = y[i];
			}
		}
		else if (key == 'R') 
		{
			eyeAngz += -5.0;
			if (eyeAngz<0.0) eyeAngz += 360.0;
			for (i = 0; i<3; i++) {
				x[i] = cv*u[0][i] + sv*u[1][i];
				y[i] = -sv*u[0][i] + cv*u[1][i];
			}
			for (i = 0; i<3; i++) {
				u[0][i] = x[i];
				u[1][i] = y[i];
			}
		}

		if (key == '1')
		{
			if (MyFovy > 45) MyFovy -= 1;
		}
		if (key == '2')
		{
			if (MyFovy < 180) MyFovy += 1;
		}
		if (key == '3')
		{
			if (MyZNear > 0) MyZNear -= 1;
		}
		if (key == '4')
		{
			if (MyZNear < MyZFar) MyZNear += 1;
			else MyZNear = MyZFar - 1;
			
		}
		if (key == '5')
		{
			if (MyZFar > MyZNear) MyZFar -= 1;
			else MyZFar = MyZNear + 1;
		}
		if (key == '6')
		{
			if (MyZFar < 200) MyZFar += 1;
		}
		if (key == '/')
		{
			glEnable(GL_LIGHTING);
			global_light = true;
		}
		if (key == '?')
		{
			glDisable(GL_LIGHTING);
			global_light = false;
		}
		if (key == 'i')
		{
			test_light->open();
		}
		if (key == 'o')
		{
			test_light->close();
		}
		if (key == 'k')
		{
			test_light1->open();
		}
		if (key == 'l')
		{
			test_light1->close();
		}
		if (key == ',')
		{
			test_light2->open();
		}
		if (key == '.')
		{
			test_light2->close();
		}
		if (key == 'm')
		{
			fan_group.material = fan_material[(++fan_material_index) % 4];
		}
		if (key == '0')
		{
			//test_light->exponent += 1;
			//if (test_light->exponent >= 127)
			//	test_light->exponent = 0;

			spot_light_angle += 0.5;
			if (spot_light_angle >= 180)
				spot_light_angle -= 360;
			test_light2->dir = { (float)sin(radius(spot_light_angle)) * (float)cos(radius(robot_elevation))
								, (float)sin(radius(robot_elevation))
								, (float)cos(radius(spot_light_angle))  * (float)cos(radius(robot_elevation)) };
		}
		if (key == 'v')
		{
			pointolite_index.m_x += 1;
			pointolite_color.m_x = abs(asin(sin(radius(pointolite_index.m_x))));
			test_light->diffuse = pointolite_color * 0.8;
			test_light->specular = pointolite_color * 0.9;

		}
		if (key == 'b')
		{
			pointolite_index.m_y += 1;
			pointolite_color.m_y = abs(asin(sin(radius(pointolite_index.m_y))));
				test_light->diffuse = pointolite_color * 0.8;
			test_light->specular = pointolite_color * 0.9;

		}
		if (key == 'n')
		{
			pointolite_index.m_z += 1;
			pointolite_color.m_z = abs(asin(sin(radius(pointolite_index.m_z))));
				test_light->diffuse = pointolite_color * 0.8;
			test_light->specular = pointolite_color * 0.9;

		}
		if (key == '+')
		{
			//test_light->exponent += 1;
			//if (test_light->exponent >= 127)
			//	test_light->exponent = 0;

			robot_elevation += 0.5;
			if (robot_elevation >= 90)
				robot_elevation -= 180;
			test_light2->dir = { (float)sin(radius(spot_light_angle)) * (float)cos(radius(robot_elevation))
				, (float)sin(radius(robot_elevation))
				, (float)cos(radius(spot_light_angle))  * (float)cos(radius(robot_elevation)) };
			printf("%f\n", robot_elevation);
		}
		if (key == '7')
		{
			sun_elevation += 0.5;
			if (sun_elevation >= 180)
				sun_elevation -= 360;
			test_light1->dir = {  (float)0, (float)sin(radius(sun_elevation)), (float)cos(radius(sun_elevation)) };

		}
		if (key == '8')
		{
			test_light2->cutoff = abs(asin(sin(radius(index_angle++)))) * 90;
			if (index_angle >= 180)
				index_angle -= 360;
		}
		if (key == '9')
		{
			test_light2->exponent += 0.5;
			if (test_light2->exponent >= 32)
				test_light2->exponent = 1;
			printf("%f\n", test_light2->exponent);
		}
	}
	glutPostRedisplay();
}

void idle_func()
{
	clock_t now = clock();
	vector_3f reverse;
	double elapsed = (double)(now - start) * 1000.0 / CLOCKS_PER_SEC;
	start = now;

	for (auto node_ptr = entity_list->first; node_ptr; node_ptr = node_ptr->next)
	{
		entity* data = (entity *)(node_ptr->data);
		data->update_entity(elapsed);
	}

	for (auto node_ptr = entity_list->first; node_ptr; node_ptr = node_ptr->next)
	{
		entity* data = (entity *)(node_ptr->data);
		if (data->can_move) {

			AABB backup_aabb = data->bounding_box;
			data->bounding_box.c.m_y -= 0.5;
			
			AABB is_hit;
			is_hit = collision_detection(data->bounding_box, data);

			data->bounding_box = backup_aabb;

			if (is_hit.r.m_y == 0)
			{
				data->bounce({0,-0.5,0});
				//printf("a %s is falling\n", typeid(*data).name());
			}
		}
	}

	for (auto node_ptr_i = entity_list->first; node_ptr_i; node_ptr_i = node_ptr_i->next)
	{
		for (auto node_ptr_j = entity_list->first; node_ptr_j; node_ptr_j = node_ptr_j->next)
		{
			if (node_ptr_i == node_ptr_j) continue;

			entity* i = (entity *)(node_ptr_i->data);
			entity* j = (entity *)(node_ptr_j->data);

			if(i->bounding_box && j->bounding_box)
			{ 
				reverse = -(~(j->bounding_box.c - i->bounding_box.c));
				if (i->can_move) {
					i->bounce(reverse*0.3);
				}
			}

		}
	}

	glutPostRedisplay();
}

void draw_floor()
{
	int   i, j;

	for (i = 0; i<50; i++)
		for (j = 0; j<50; j++) 
		{
			if ((i + j) % 2 == 0) 
			{
				glMaterialfv(GL_FRONT, GL_AMBIENT, vector_4f(0.3, 0.3, 0.3, 1));
				glMaterialfv(GL_FRONT, GL_SPECULAR, vector_4f(vector_3f(0.9, 0.8, 0.7) *0.9, 1));
				glMaterialfv(GL_FRONT, GL_DIFFUSE, vector_4f(vector_3f(0.9,0.8,0.7) *0.8, 1));
				glMaterialf(GL_FRONT, GL_SHININESS, 32);
				glColor3f(0.9, 0.8, 0.7);

			}
			else
			{
				glMaterialfv(GL_FRONT, GL_AMBIENT, vector_4f(0.1, 0.1, 0.1, 1));
				glMaterialfv(GL_FRONT, GL_SPECULAR, vector_4f(vector_3f(0.3, 0.2, 0.1) *0.9, 1));
				glMaterialfv(GL_FRONT, GL_DIFFUSE, vector_4f(vector_3f(0.3, 0.2, 0.1) *0.8, 1));
				glMaterialf(GL_FRONT, GL_SHININESS, 32);
				glColor3f(0.3, 0.2, 0.1);
			}
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			glVertex3f(i, 0.0, j);
			glVertex3f(i, 0.0, j + 1);
			glVertex3f(i + 1, 0.0, j + 1);
			glVertex3f(i + 1, 0.0, j);
			glEnd();
		}
	glBegin(GL_POLYGON);
	glVertex3f(0, -0.5, 0);
	glVertex3f(0, -0.5, 50);
	glVertex3f(0, 0.0, 50);
	glVertex3f(0, 0.0, 0);
	glEnd();
}

void dfs_draw(struct tree_node<base *> *root)
{
	if (root) 
	{
		auto current = root->son;

		for (auto iter = current->first; iter; iter = iter->next)
		{
			glPushMatrix();
			iter->data->data->draw();
			dfs_draw(iter->data);
			glPopMatrix();
		}
	}

}

void dfs_draw_light(struct tree_node<base *> *root)
{
	if (root)
	{
		auto current = root->son;

		for (auto iter = current->first; iter; iter = iter->next)
		{
			glPushMatrix();
			iter->data->data->draw_light();
			dfs_draw_light(iter->data);
			glPopMatrix();
		}
	}
}

void init_scene()
{
	scene_graph = { create_tree_node<base *>((base *)new group("root")) };
	auto iterator = add_tree_node(scene_graph.root, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 25,0,25 }));
	scene_node = add_tree_node(iterator, (base *)new group("scene"));
	// light 0
	iterator = add_tree_node(scene_node, (base *)new group("light"));
	iterator = add_tree_node(iterator, (base *)new trans({ 0,0,0 }, { 2,2,2 }, { 0,0,0 }));
	light_node = add_tree_node(iterator, (base *)test_light);
	iterator = add_tree_node(iterator, (base *)test_light1);
	
	test_light->diffuse = { 0.8f,0.0f,0.0f,1.0f };
	test_light->specular = { 0.9f,0.0f,0.0f,1.0f };

	test_light1->diffuse = { 0.8f,0.8f,0.0f,1.0f };
	test_light1->specular = {0.9f,0.9f,0.0f,1.0f};
	
	// fan
	iterator = add_tree_node(scene_node, (base *)new group("fan"));
	fan_node = add_tree_node(iterator, (base *)new trans({ 0,180,0 }, { 1,1,1 }, { 0,0,-10 }));
	iterator = add_tree_node(fan_node, (base *)&fan_group);
	iterator = add_tree_node(iterator, (base *)new polyhedra("kanna/kanna.obj"));

	//billboard-flower
	iterator = add_tree_node(scene_node, (base *)new group("flower"));
	iterator = add_tree_node(iterator, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 10,0,10 }));
	iterator = add_tree_node(iterator, (base *)(new group("flower_color", { 1, 1, 1 }))->setTexture({"sapling_acacia.bmp"}, {true}));
	iterator = add_tree_node(iterator, (base *)(flower = new billboard(10,10)));

	entity_list = create_list<entity *>();
}

void init_robot()
{
	robot_ptr = add_tree_node(scene_node, (base *)new group("robot"));
	/*create body*/
	auto iterator = add_tree_node(robot_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,9,0 }));
	body_ptr = add_tree_node(iterator, (base *)new group("body"));	
	iterator = add_tree_node(body_ptr, (base *)new trans({ 0,0,0 }, { 4,4,1.5 }, { 0,0,0 }));
	iterator = add_tree_node(iterator, (base *)(new group("body_color", { 1, 1, 1 }))->setTexture({ "diamond_ore.bmp" }, { false }));
	cube_node = add_tree_node(iterator, (base *)new cube());
	

	/* create upper body*/
		iterator = add_tree_node(body_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,2,0 }));
		u_body_ptr = add_tree_node(iterator, (base *)new group("upper_body"));
		
		/* create head */
			iterator = add_tree_node(u_body_ptr, (base *)new trans({ 0,0,0 }, { 1.5,1.5,1.5 }, { 0,0.75,0 }));
			head_ptr = add_tree_node(iterator, (base *)new group("head"));
			iterator = add_tree_node(head_ptr, (base *)new group("head_color", { 0,0,1 }));
			ball_node = add_tree_node(iterator, (base *)new ball());
		/* create head light*/
			iterator = add_tree_node(head_ptr, (base *)new trans({ 0,0,0 }, {1,1,1 }, { 0,3,0 }));
			iterator = add_tree_node(iterator, (base *)test_light2);
			test_light2->diffuse = { 0.8f,0.8f,0.8f,1.0f };
			test_light2->specular = { 0.9f,0.9f,0.9f,1.0f };

		/* create left arm */
			l_shoulder_ptr = add_tree_node(u_body_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 2.5,0,0 }));
			iterator = add_tree_node(l_shoulder_ptr, (base *)new group("left_shoulder"));
			iterator = add_tree_node(iterator, (base *)new group("shoulder_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);

			l_u_arm_ptr = add_tree_node(l_shoulder_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,0,0 }));
	auto	iter_node = add_tree_node(l_u_arm_ptr, (base *)new group("left_upper_arm"));
			iterator = add_tree_node(iter_node, (base *)new group("arm_color", { 1,0,0 }));
			iterator = add_tree_node(iterator, (base *)new trans({ 90,0,0 }, { 1,1,3 }, { 0,0,0 }));
			prism_node = add_tree_node(iterator, (base *)new prism());

			l_elbow_ptr = add_tree_node(iter_node, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,-3,0 }));
			iterator = add_tree_node(l_elbow_ptr, (base *)new group("left_elbow"));
			iterator = add_tree_node(iterator, (base *)new group("elbow_color", { 1,1,1 }));
			add_subtree(iterator, ball_node);

			l_forearm_ptr = add_tree_node(l_elbow_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,0,0 }));
			iterator = add_tree_node(l_forearm_ptr, (base *)new group("left_forearm"));
			iterator = add_tree_node(iterator, (base *)new group("arm_color", { 1,0,0 }));
			iterator = add_tree_node(iterator, (base *)new trans({ 90,0,0 }, { 1,1,3 }, { 0,0,0 }));
			add_subtree(iterator, prism_node);

			l_hand_ptr = add_tree_node(l_forearm_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,-3,0 }));
			iterator = add_tree_node(l_hand_ptr, (base *)new group("left_hand"));
			iterator = add_tree_node(l_hand_ptr, (base *)new group("hand_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);

		/* create right arm */
			r_shoulder_ptr = add_tree_node(u_body_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { -2.5,0,0 }));
			iterator = add_tree_node(r_shoulder_ptr, (base *)new group("right_shoulder"));
			iterator = add_tree_node(iterator, (base *)new group("shoulder_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);

			r_u_arm_ptr = add_tree_node(r_shoulder_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,0,0 }));
			iter_node = add_tree_node(r_u_arm_ptr, (base *)new group("right_upper_arm"));
			iterator = add_tree_node(iter_node, (base *)new group("arm_color", { 1,0,0 }));
			iterator = add_tree_node(iterator, (base *)new trans({ 90,0,0 }, { 1,1,3 }, { 0,0,0 }));
			prism_node = add_tree_node(iterator, (base *)new prism());

			r_elbow_ptr = add_tree_node(iter_node, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,-3,0 }));
			iterator = add_tree_node(r_elbow_ptr, (base *)new group("right_elbow"));
			iterator = add_tree_node(iterator, (base *)new group("elbow_color", { 1,1,1 }));
			add_subtree(iterator, ball_node);

			r_forearm_ptr = add_tree_node(r_elbow_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,0,0 }));
			iterator = add_tree_node(r_forearm_ptr, (base *)new group("right_forearm"));
			iterator = add_tree_node(iterator, (base *)new group("arm_color", { 1,0,0 }));
			iterator = add_tree_node(iterator, (base *)new trans({ 90,0,0 }, { 1,1,3 }, { 0,0,0 }));
			add_subtree(iterator, prism_node);

			r_hand_ptr = add_tree_node(r_forearm_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,-3,0 }));
			iterator = add_tree_node(r_hand_ptr, (base *)new group("right_hand"));
			iterator = add_tree_node(r_hand_ptr, (base *)new group("hand_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);

	/* create lower body*/
		iterator = add_tree_node(body_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,-2,0 }));
		lo_body_ptr = add_tree_node(iterator, (base *)new group("lower_body"));
			
		/* create left lag */
			l_hip_ptr = add_tree_node(lo_body_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 1,0,0 }));
			iterator = add_tree_node(l_hip_ptr, (base *)new group("left_hip"));
			iterator = add_tree_node(l_hip_ptr, (base *)new group("hip_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);

			l_thigh_ptr = add_tree_node(l_hip_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,0,0 }));
			iter_node = add_tree_node(l_thigh_ptr, (base *)new group("left_thigh"));
			iterator = add_tree_node(iter_node, (base *)new group("thigh_color", { 1,0,0 }));
			iterator = add_tree_node(iterator, (base *)new trans({ 90,0,0 }, { 1,1,3 }, { 0,0,0 }));
			prism_node = add_tree_node(iterator, (base *)new prism());

			l_knee_ptr = add_tree_node(iter_node, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,-3,0 }));
			iterator = add_tree_node(l_knee_ptr, (base *)new group("left_knee"));
			iterator = add_tree_node(iterator, (base *)new group("kenn_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);

			l_calf_ptr = add_tree_node(l_knee_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,0,0 }));
			iterator = add_tree_node(l_calf_ptr, (base *)new group("left_calf"));
			iterator = add_tree_node(iterator, (base *)new group("calf_color", { 1,0,0 }));
			iterator = add_tree_node(iterator, (base *)new trans({ 90,0,0 }, { 1,1,3 }, { 0,0,0 }));
			add_subtree(iterator, prism_node);

			l_foot_ptr = add_tree_node(l_calf_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,-3,0 }));
			iterator = add_tree_node(l_foot_ptr, (base *)new group("left_foot"));
			iterator = add_tree_node(iterator, (base *)new group("foot_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);

		/* create right lag */
			r_hip_ptr = add_tree_node(lo_body_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { -1,0,0 }));
			iterator = add_tree_node(r_hip_ptr, (base *)new group("right_hip"));
			iterator = add_tree_node(r_hip_ptr, (base *)new group("hip_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);

			r_thigh_ptr = add_tree_node(r_hip_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,0,0 }));
			iter_node = add_tree_node(r_thigh_ptr, (base *)new group("right_thigh"));
			iterator = add_tree_node(iter_node, (base *)new group("thigh_color", { 1,0,0 }));
			iterator = add_tree_node(iterator, (base *)new trans({ 90,0,0 }, { 1,1,3 }, { 0,0,0 }));
			prism_node = add_tree_node(iterator, (base *)new prism());

			r_knee_ptr = add_tree_node(iter_node, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,-3,0 }));
			iterator = add_tree_node(r_knee_ptr, (base *)new group("right_knee"));
			iterator = add_tree_node(iterator, (base *)new group("kenn_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);

			r_calf_ptr = add_tree_node(r_knee_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,0,0 }));
			iterator = add_tree_node(r_calf_ptr, (base *)new group("right_calf"));
			iterator = add_tree_node(iterator, (base *)new group("calf_color", { 1,0,0 }));
			iterator = add_tree_node(iterator, (base *)new trans({ 90,0,0 }, { 1,1,3 }, { 0,0,0 }));
			add_subtree(iterator, prism_node);

			r_foot_ptr = add_tree_node(r_calf_ptr, (base *)new trans({ 0,0,0 }, { 1,1,1 }, { 0,-3,0 }));
			iterator = add_tree_node(r_foot_ptr, (base *)new group("left_foot"));
			iterator = add_tree_node(iterator, (base *)new group("foot_color", { 1,1,0 }));
			add_subtree(iterator, ball_node);
	/*----------------*/
	robot_entity = new robot(
		(trans *)(robot_ptr->son->first->data->data), 
		(trans *)(l_shoulder_ptr->data),
		(trans *)(l_u_arm_ptr->son->first->data->data),
		(trans *)(l_elbow_ptr->data),
		(trans *)(l_forearm_ptr->son->first->data->data),
		(trans *)(l_hand_ptr->data),

		(trans *)(r_shoulder_ptr->data),
		(trans *)(r_u_arm_ptr->son->first->data->data),
		(trans *)(r_elbow_ptr->data),
		(trans *)(r_forearm_ptr->son->first->data->data),
		(trans *)(r_hand_ptr->data),

		(trans *)(l_hip_ptr->data),
		(trans *)(l_thigh_ptr->son->first->data->data),
		(trans *)(l_knee_ptr->data),
		(trans *)(l_calf_ptr->son->first->data->data),
		(trans *)(l_foot_ptr->data),

		(trans *)(r_hip_ptr->data),
		(trans *)(r_thigh_ptr->son->first->data->data),
		(trans *)(r_knee_ptr->data),
		(trans *)(r_calf_ptr->son->first->data->data),
		(trans *)(r_foot_ptr->data),
		{0.0f,0.0f,1.0f}
	);
	robot_entity->bounding_box = { {25,0,25},{2,6.5,1.5} };
}

void init_entity()
{
	add_data(entity_list, (entity *)robot_entity);
	add_data(entity_list, (entity *)new ground_floor());
	add_data(entity_list, (entity *)new obstacle({ (((trans *)(fan_node->data))->m_shift + vector_3f(0,6,0)) ,{2,9,2} }));

	/*
	auto iterator = add_tree_node(scene_node, (base *)new group("udo1"));
	auto udo_node = add_tree_node(iterator, (base *)new trans({ 0,0,0 }, { 3,3,3 }, { -10,10,10 }));
	iterator = add_tree_node(udo_node, (base *)new group("udo1_color", { 1, 1, 1 }));
	add_subtree(iterator, cube_node);
	entity * udo_entity = (entity *)new obstacle({ (((trans *)(udo_node->data))->m_shift + vector_3f(0,0,0)) ,{ 1.5,1.5,1.5 } });
	((obstacle*)udo_entity)->set_coord((trans *)(udo_node->data));
	udo_entity->can_move = true;
	add_data(entity_list, udo_entity);
	*/
}

AABB collision_detection(AABB test, entity *check)
{
	for (auto node_ptr = entity_list->first; node_ptr; node_ptr = node_ptr->next)
	{
		entity* data = (entity *)(node_ptr->data);
		if (check == data) continue;
		if (test && data->bounding_box) {
			//printf("%s is hit\n",typeid(*check).name());
			return data->bounding_box;			
		}
	}
	return { {0,0,0},{0,0,0} };
}

void add_udo_cube()
{
	int x = rand() % 50 - 25;
	int z = rand() % 50 - 25;
	//float R = (rand() % 255) / 255.0;
	//float G = (rand() % 255) / 255.0;
	//float B = (rand() % 255) / 255.0;

	auto iterator = add_tree_node(scene_node, (base *)new group("udo1"));
	auto udo_node = add_tree_node(iterator, (base *)new trans({ 0,0,0 }, { 3.f,3.f,3.f }, { (float)x,10.f,(float)z }));
	iterator = add_tree_node(udo_node, (base *)(new group("udo1_color")));

	auto G1_inside = add_tree_node(iterator, (base *)(new group("udo1_inside", { 1, 1, 1 }))->setTexture({ "cobblestone_mossy.bmp" }, { true }));
	auto G2_inside = add_tree_node(iterator, (base *)(new group("udo2_inside", { 0.9f, 0.9f, 0.9f }))->setTexture({ (std::string("0/") + std::to_string(rand()%7) + ".bmp").c_str() }, { false}));
	add_subtree(G1_inside, cube_node);
	add_subtree(G2_inside, cube_node);


	entity * udo_entity = (entity *)new obstacle({ (((trans *)(udo_node->data))->m_shift + vector_3f(0,0,0)) ,{ 1.5,1.5,1.5 } });
	((obstacle*)udo_entity)->set_coord((trans *)(udo_node->data));
	udo_entity->can_move = true;
	add_data(entity_list, udo_entity);
}
float waterfallval = 0;

void draw_waterfall()
{
	waterfallval += 0.01;
	if (waterfallval > 1)waterfallval = 0;
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterfall_texture);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, waterfallval, 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glBegin(GL_POLYGON);
			glTexCoord2f(1, 0);
			glVertex3f(5.0, -10.0, 50.0);
			glTexCoord2f(1, 1);
			glVertex3f(5.0, 0.0, 50.0);
			glTexCoord2f(0, 1);
			glVertex3f(0.0, 0.0, 50.0);
			glTexCoord2f(0, 0);
			glVertex3f(0.0, -10.0, 50.0);
		glEnd();
		glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
}
int keyF = 0;
void draw_sae()
{
	keyF = (keyF + 1) % 120;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, keyFrame[keyF / 5]);
	glTranslatef(10, 2, 20);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3f(-5.0 * u[0][0], -1.0, -5.0 * u[0][2]);
	glTexCoord2f(1, 0);
	glVertex3f(+5.0  * u[0][0], -1.0, +5.0 * u[0][2]);
	glTexCoord2f(1, 1);
	glVertex3f(+5.0  * u[0][0], 9.0, +5.0  * u[0][2]);
	glTexCoord2f(0, 1);
	glVertex3f(-5.0* u[0][0], 9.0, -5.0 * u[0][2]);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}