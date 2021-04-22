#pragma once

class robot :
	public entity
{
public:
	robot();
	robot(trans *r_co, trans *l_s, trans *l_u, trans *l_e, trans *l_f, trans *l_ha
					, trans *r_s, trans *r_u, trans *r_e, trans *r_f, trans *r_ha
		, trans *l_h, trans *l_t, trans *l_k, trans *l_c, trans *l_foo
		, trans *r_h, trans *r_t, trans *r_k, trans *r_c, trans *r_foo, vector_3f f);
	~robot();
	void update(double t);
	void bounce(vector_3f S);
	void set_move_x(vector_3f phy);
	void set_shift(vector_3f sce);
	vector_3f get_location();

	float walk_state;
	float jump_state;

	trans * robot_coord;
	trans * l_shoulder;
	trans * l_u_arm;
	trans * l_elbow;
	trans * l_forearm;
	trans * l_hand;

	trans * r_shoulder;
	trans * r_u_arm;
	trans * r_elbow;
	trans * r_forearm;
	trans * r_hand;

	trans * l_hip;
	trans * l_thigh;
	trans * l_knee;
	trans * l_calf;
	trans * l_foot;

	trans * r_hip;
	trans * r_thigh;
	trans * r_knee;
	trans * r_calf;
	trans * r_foot;

	vector_3f facing;
	vector_3f move_x;

	bool is_walking;
	bool is_jumping;
	bool is_running;
private:
	bool first_step;
	bool second_step;
	
};

