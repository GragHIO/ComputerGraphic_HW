#pragma once
#ifndef MAIN_H
#define MAIN_H

void myinit();
void draw_axes();
void display_scene();
void draw_view();
void make_view(style_t x);
void make_projection(style_t x);
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void idle_func();
void draw_floor();
void dfs_draw(struct tree_node<base *> *root);
void dfs_draw_light(struct tree_node<base *> *root);
void init_scene();
void init_robot();
void init_entity();
AABB collision_detection(AABB test, entity *check);
void add_udo_cube();
void draw_waterfall();
void draw_sae();
#endif