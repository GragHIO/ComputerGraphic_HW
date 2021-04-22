#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

extern int width, height;
extern float   eye[3];
extern float   u[3][3];
extern tree<base *> scene_graph;
extern tree_node<base *> *scene_node;
extern tree_node<base *> *robot_ptr;
extern tree_node<base *> *body_ptr;
extern tree_node<base *> *lo_body_ptr;
extern tree_node<base *> *u_body_ptr;
extern tree_node<base *> *head_ptr;
extern tree_node<base *> *l_u_arm_ptr;
extern tree_node<base *> *l_shoulder_ptr;
extern tree_node<base *> *l_elbow_ptr;
extern tree_node<base *> *l_forearm_ptr;
extern tree_node<base *> *l_hand_ptr;

extern tree_node<base *> *r_u_arm_ptr;
extern tree_node<base *> *r_shoulder_ptr;
extern tree_node<base *> *r_elbow_ptr;
extern tree_node<base *> *r_forearm_ptr;
extern tree_node<base *> *r_hand_ptr;

extern tree_node<base *> *l_hip_ptr;
extern tree_node<base *> *l_thigh_ptr;
extern tree_node<base *> *l_knee_ptr;
extern tree_node<base *> *l_calf_ptr;
extern tree_node<base *> *l_foot_ptr;

extern tree_node<base *> *r_hip_ptr;
extern tree_node<base *> *r_thigh_ptr;
extern tree_node<base *> *r_knee_ptr;
extern tree_node<base *> *r_calf_ptr;
extern tree_node<base *> *r_foot_ptr;

extern tree_node<base *> *cube_node;
extern tree_node<base *> *ball_node;
extern tree_node<base *> *prism_node;
extern tree_node<base *> *fan_node;
extern tree_node<base *> *light_node;

extern billboard *flower;
extern std::map<std::string, unsigned int> *loadded_texture;
extern struct list_head<entity *> *entity_list;
extern robot *robot_entity;
extern clock_t start;
extern float MyFovy, MyAspect, MyZNear, MyZFar;
extern bool global_light;
enum style_t {X_axis, Y_axis, Z_axis, Free_axis, Three_axis};
#endif