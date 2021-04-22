#include "HIO_include.h"

extern int width = 1000, height = 1000;
extern float   eye[3] = {0,0,0};
extern float   u[3][3] = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{ 0.0,0.0,1.0 } };
extern tree<base *> scene_graph				= {NULL};
extern tree_node<base *> *scene_node		= NULL;
extern tree_node<base *> *robot_ptr			= NULL;
extern tree_node<base *> *body_ptr			= NULL;
extern tree_node<base *> *lo_body_ptr		= NULL;
extern tree_node<base *> *u_body_ptr		= NULL;
extern tree_node<base *> *head_ptr			= NULL;
extern tree_node<base *> *l_u_arm_ptr		= NULL;
extern tree_node<base *> *l_shoulder_ptr	= NULL;
extern tree_node<base *> *l_elbow_ptr		= NULL;
extern tree_node<base *> *l_forearm_ptr		= NULL;
extern tree_node<base *> *l_hand_ptr		= NULL;

extern tree_node<base *> *r_u_arm_ptr		= NULL;
extern tree_node<base *> *r_shoulder_ptr	= NULL;
extern tree_node<base *> *r_elbow_ptr		= NULL;
extern tree_node<base *> *r_forearm_ptr		= NULL;
extern tree_node<base *> *r_hand_ptr		= NULL;

extern tree_node<base *> *l_hip_ptr			= NULL;
extern tree_node<base *> *l_thigh_ptr		= NULL;
extern tree_node<base *> *l_knee_ptr		= NULL;
extern tree_node<base *> *l_calf_ptr		= NULL;
extern tree_node<base *> *l_foot_ptr		= NULL;

extern tree_node<base *> *r_hip_ptr			= NULL;
extern tree_node<base *> *r_thigh_ptr		= NULL;
extern tree_node<base *> *r_knee_ptr		= NULL;
extern tree_node<base *> *r_calf_ptr		= NULL;
extern tree_node<base *> *r_foot_ptr		= NULL;

extern tree_node<base *> *cube_node			= NULL;
extern tree_node<base *> *ball_node			= NULL;
extern tree_node<base *> *prism_node		= NULL;
extern tree_node<base *> *fan_node			= NULL;
extern tree_node<base *> *light_node		= NULL;
extern billboard		 *flower			= NULL;
extern std::map<std::string, unsigned int> *loadded_texture = new std::map<std::string, unsigned int>();
extern struct list_head<entity *> *entity_list = NULL;
extern robot *robot_entity = NULL;
extern clock_t start = 0;
extern float MyFovy = 90.0, MyAspect = 1.0, MyZNear = 1.5, MyZFar = 50.0;
extern bool global_light = false;