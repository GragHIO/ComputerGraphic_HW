#include "HIO_include.h"

robot::robot()
{
}

robot::robot(trans *r_co, trans *l_s, trans *l_u, trans *l_e, trans *l_f, trans *l_ha
	, trans *r_s, trans *r_u, trans *r_e, trans *r_f, trans *r_ha
	, trans *l_h, trans *l_t, trans *l_k, trans *l_c, trans *l_foo
	, trans *r_h, trans *r_t, trans *r_k, trans *r_c, trans *r_foo, vector_3f f)
{
	robot_coord = r_co;
	l_shoulder	= l_s;
	l_u_arm		= l_u;
	l_elbow		= l_e;
	l_forearm	= l_f;
	l_hand		= l_ha;

	r_shoulder	= r_s;
	r_u_arm		= r_u;
	r_elbow		= r_e;
	r_forearm	= r_f;
	r_hand		= r_ha;
		
	l_hip		= l_h;
	l_thigh		= l_t;
	l_knee		= l_k;
	l_calf		= l_c;
	l_foot		= l_f;

	r_hip		= r_h;
	r_thigh		= r_t;
	r_knee		= r_k;
	r_calf		= r_c;
	r_foot		= r_f;

	facing = f;
	move_x = robot_coord->m_shift;	//¬ö¿ý®y¼Ð
	walk_state = 0;
	jump_state = 0;
	first_step = false;
	second_step = false;
	is_walking = false;
	is_jumping = false;
	is_running = false;
	can_move = true;
}


robot::~robot()
{
}

void robot::update(double t)
{
	if (is_walking == true)
	{
		walk_state += 0.1;
		if (walk_state < 1)
		{
			first_step = false;
			second_step = false;
			l_hip->m_angle.m_x = -45 * walk_state;
			l_knee->m_angle.m_x = 45 * walk_state;

			l_shoulder->m_angle.m_x = 45 * walk_state;
			r_shoulder->m_angle.m_x = -45 * walk_state;
		}
		else if (walk_state < 2)
		{
			l_hip->m_angle.m_x = -45;
			l_knee->m_angle.m_x = 45;
			r_hip->m_angle.m_x = 35 * (walk_state - 1);
			set_shift(move_x + (facing * 6 * sin(radius(r_hip->m_angle.m_x))));
			set_shift({
				robot_coord->m_shift.m_x,
				(float)(move_x.m_y - 6 + 6 * cos(radius(r_hip->m_angle.m_x))),
				robot_coord->m_shift.m_z
			});
			first_step = false;
			second_step = false;

		}
		else if (walk_state < 3)
		{
			if (!first_step)
			{
				first_step = true;
				second_step = false;
				set_move_x(move_x + (facing * 6 * sin(radius(31.5 * (walk_state - 1)))));
			}
			l_hip->m_angle.m_x = -45 * (1 - (walk_state - 2));
			l_knee->m_angle.m_x = 45 * (1 - (walk_state - 2));
			set_shift(move_x + (facing * 6 * sin(radius(22.5 * ((walk_state - 2))))));
			set_shift({
				robot_coord->m_shift.m_x,
				(float)(move_x.m_y - 6 + 6 * cos(radius(31.5 * (1 - (walk_state - 2))))),
				robot_coord->m_shift.m_z,
			});
			r_hip->m_angle.m_x = 35 * (1 - (walk_state - 2));

			l_shoulder->m_angle.m_x = 45 * (1 - (walk_state - 2));
			r_shoulder->m_angle.m_x = -45 * (1 - (walk_state - 2));


		}
		else if (walk_state < 4)
		{
			if (!second_step)
			{
				second_step = true;
				set_move_x(move_x + (facing * 6 * sin(radius(22.5 * ((walk_state - 2))))));
			}
			r_hip->m_angle.m_x = -45 * (walk_state - 3);
			r_knee->m_angle.m_x = 45 * (walk_state - 3);

			l_shoulder->m_angle.m_x = -45 * (walk_state - 3);
			r_shoulder->m_angle.m_x = 45 * (walk_state - 3);
		}
		else if (walk_state < 5)
		{
			r_hip->m_angle.m_x = -45;
			r_knee->m_angle.m_x = 45;
			l_hip->m_angle.m_x = 35 * (walk_state - 4);
			set_shift(move_x + (facing * 6 * sin(radius(l_hip->m_angle.m_x))));
			set_shift({
				robot_coord->m_shift.m_x,
				(float)(move_x.m_y - 6 + 6 * cos(radius(l_hip->m_angle.m_x))),
				robot_coord->m_shift.m_z
			});
			first_step = false;
			second_step = false;


		}
		else if (walk_state < 6)
		{
			if (!second_step)
			{
				second_step = true;
				move_x += (facing * 6 * sin(radius(31.5 * (walk_state - 4))));
			}
			r_hip->m_angle.m_x = -45 * (1 - (walk_state - 5));
			r_knee->m_angle.m_x = 45 * (1 - (walk_state - 5));
			set_shift(move_x + (facing * 6 * sin(radius(22.5 * ((walk_state - 5))))));
			set_shift({
				robot_coord->m_shift.m_x,
				(float)(move_x.m_y - 6 + 6 * cos(radius(31.5 * (1 - (walk_state - 5))))),
				robot_coord->m_shift.m_z,
			});
			l_hip->m_angle.m_x = 35 * (1 - (walk_state - 5));

			l_shoulder->m_angle.m_x = -45 * (1 - (walk_state - 5));
			r_shoulder->m_angle.m_x = 45 * (1 - (walk_state - 5));
		}
		else
		{
			move_x += (facing * 6 * sin(radius(22.5 * ((walk_state - 5)))));
			first_step = false;
			second_step = false;
			walk_state = 0;
			is_walking = false;
		}
	}
	if (is_jumping == true)
	{
		jump_state += 0.1;
		if (jump_state < 1)
		{
			l_hip->m_angle.m_x = -30 * jump_state;
			l_knee->m_angle.m_x = 60 * jump_state;
			r_hip->m_angle.m_x = -30 * jump_state;
			r_knee->m_angle.m_x = 60 * jump_state;
			set_shift({
				robot_coord->m_shift.m_x,
				(float)(move_x.m_y - 6 + 6 * cos(radius(r_hip->m_angle.m_x))),
				robot_coord->m_shift.m_z
			});
		}
		else if (jump_state < 2)
		{
			l_hip->m_angle.m_x = -30 * (2 - jump_state);
			l_knee->m_angle.m_x = 60 * (2 - jump_state);
			r_hip->m_angle.m_x = -30 * (2 - jump_state);
			r_knee->m_angle.m_x = 60 * (2 - jump_state);
			set_move_x({
				robot_coord->m_shift.m_x,
				//(float)(move_x.m_y - 6 + 6 * cos(radius(r_hip->m_angle.m_x))),
				(float)(robot_coord->m_shift.m_y + (2 - jump_state) * 4),
				robot_coord->m_shift.m_z
			});
			set_shift({
				robot_coord->m_shift.m_x,
				//(float)(move_x.m_y - 6 + 6 * cos(radius(r_hip->m_angle.m_x))),
				(float)(robot_coord->m_shift.m_y + (2 - jump_state) * 4),
				robot_coord->m_shift.m_z
			});
		}
		else
		{
			is_jumping = false;
			jump_state = 0;
		}
	}
	if (is_running == true)
	{
		walk_state += 0.2;
		if (walk_state < 1)
		{
			first_step = false;
			second_step = false;
			l_hip->m_angle.m_x = -45 * walk_state;
			l_knee->m_angle.m_x = 45 * walk_state;

			l_shoulder->m_angle.m_x = 45 * walk_state;
			r_shoulder->m_angle.m_x = -45 * walk_state;
		}
		else if (walk_state < 2)
		{
			l_hip->m_angle.m_x = -45;
			l_knee->m_angle.m_x = 45;
			r_hip->m_angle.m_x = 35 * (walk_state - 1);
			set_shift(move_x + (facing * 6 * sin(radius(r_hip->m_angle.m_x))));
			set_shift({
				robot_coord->m_shift.m_x,
				(float)(move_x.m_y - 6 + 6 * cos(radius(r_hip->m_angle.m_x))),
				robot_coord->m_shift.m_z
			});
			first_step = false;
			second_step = false;

		}
		else if (walk_state < 3)
		{
			if (!first_step)
			{
				first_step = true;
				second_step = false;
				set_move_x(move_x + (facing * 6 * sin(radius(31.5 * (walk_state - 1)))));
			}
			l_hip->m_angle.m_x = -45 * (1 - (walk_state - 2));
			l_knee->m_angle.m_x = 45 * (1 - (walk_state - 2));
			set_shift(move_x + (facing * 6 * sin(radius(22.5 * ((walk_state - 2))))));
			set_shift({
				robot_coord->m_shift.m_x,
				(float)(move_x.m_y - 6 + 6 * cos(radius(31.5 * (1 - (walk_state - 2))))),
				robot_coord->m_shift.m_z,
			});
			r_hip->m_angle.m_x = 35 * (1 - (walk_state - 2));

			l_shoulder->m_angle.m_x = 45 * (1 - (walk_state - 2));
			r_shoulder->m_angle.m_x = -45 * (1 - (walk_state - 2));
		}
		else if (walk_state < 4)
		{
			if (!second_step)
			{
				second_step = true;
				set_move_x(move_x + (facing * 6 * sin(radius(22.5 * ((walk_state - 2))))));
			}
			r_hip->m_angle.m_x = -45 * (walk_state - 3);
			r_knee->m_angle.m_x = 45 * (walk_state - 3);

			l_shoulder->m_angle.m_x = -45 * (walk_state - 3);
			r_shoulder->m_angle.m_x = 45 * (walk_state - 3);
		}
		else if (walk_state < 5)
		{
			r_hip->m_angle.m_x = -45;
			r_knee->m_angle.m_x = 45;
			l_hip->m_angle.m_x = 35 * (walk_state - 4);
			set_shift(move_x + (facing * 6 * sin(radius(l_hip->m_angle.m_x))));
			set_shift({
				robot_coord->m_shift.m_x,
				(float)(move_x.m_y - 6 + 6 * cos(radius(l_hip->m_angle.m_x))),
				robot_coord->m_shift.m_z
			});
			first_step = false;
			second_step = false;
		}
		else if (walk_state < 6)
		{
			if (!second_step)
			{
				second_step = true;
				move_x += (facing * 6 * sin(radius(31.5 * (walk_state - 4))));
			}
			r_hip->m_angle.m_x = -45 * (1 - (walk_state - 5));
			r_knee->m_angle.m_x = 45 * (1 - (walk_state - 5));
			set_shift(move_x + (facing * 6 * sin(radius(22.5 * ((walk_state - 5))))));
			set_shift({
				robot_coord->m_shift.m_x,
				(float)(move_x.m_y - 6 + 6 * cos(radius(31.5 * (1 - (walk_state - 5))))),
				robot_coord->m_shift.m_z,
			});
			l_hip->m_angle.m_x = 35 * (1 - (walk_state - 5));

			l_shoulder->m_angle.m_x = -45 * (1 - (walk_state - 5));
			r_shoulder->m_angle.m_x = 45 * (1 - (walk_state - 5));
		}
		else
		{
			move_x += (facing * 6 * sin(radius(22.5 * ((walk_state - 5)))));
			first_step = false;
			second_step = false;
			walk_state = 0;
			is_running = false;
		}
	}
}

void robot::bounce(vector_3f S)
{
	move_x += S;
	robot_coord->m_shift += S;
}

void robot::set_move_x(vector_3f phy)
{
	vector_3f temp_n = move_x;
	move_x = move_x + (phy - move_x) * 1.1;

	AABB test_collision = bounding_box;
	AABB is_hit;
	test_collision.c = get_location();

	is_hit = collision_detection(bounding_box, this);

	if (is_hit.r.m_y != 0)
		move_x = temp_n;
	else
		move_x = phy;

}

void robot::set_shift(vector_3f sce)
{
	//robot_coord->m_shift = sce;
	vector_3f temp_n = robot_coord->m_shift;
	robot_coord->m_shift = robot_coord->m_shift + (sce - robot_coord->m_shift) * 1.1;

	AABB test_collision = bounding_box;
	AABB is_hit;
	test_collision.c = get_location();
	is_hit = collision_detection(bounding_box, this);

	if (is_hit.r.m_y != 0)
		robot_coord->m_shift = temp_n;
	else
		robot_coord->m_shift = sce;
}

vector_3f robot::get_location()
{
	//printf("%f\n", move_x.m_y);
	return vector_3f(
		robot_coord->m_shift.m_x,
		move_x.m_y - 2.0,
		robot_coord->m_shift.m_z
		);
}




