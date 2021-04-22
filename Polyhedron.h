#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef POLYHEDRON_H
#define POLYHEDRON_H

typedef enum {
	OBJ_KD,
	OBJ_KA,
	OBJ_KE,
	OBJ_KS,
	OBJ_NS,
	OBJ_KN,
	OBJ_PROP_COUNT
} polyhedron_data_t;

typedef unsigned int index_t;
extern GLuint textureEmpty;
typedef struct OBJ_PROP
{
	char        *str;
	int          opt;
	unsigned int map;

	float c[4];
	float o[3];
	float s[3];
} obj_prop;

typedef struct OBJ_MTRL
{
	char *name;
	obj_prop kv[polyhedron_data_t::OBJ_PROP_COUNT];
} obj_mtrl;

typedef struct OBJ_VERT
{
	float u[3];
	float n[3];
	float t[2];
	float v[3];
} obj_vert;

typedef struct OBJ_POLY
{
	index_t vi[3];
} obj_poly;

typedef struct OBJ_LINE
{
	index_t vi[2];
} obj_line;

typedef struct OBJ_SURF
{
	int mi;

	int pc;
	int pm;
	int lc;
	int lm;

	unsigned int pibo;
	unsigned int libo;

	obj_poly *pv;
	obj_line *lv;
} obj_surf;

typedef struct POLYHEDRON
{
	unsigned int vao;
	unsigned int vbo;

	int mc;
	int mm;
	int vc;
	int vm;
	int sc;
	int sm;

	int uloc;
	int nloc;
	int tloc;
	int vloc;

	int cloc[OBJ_PROP_COUNT];
	int oloc[OBJ_PROP_COUNT];
	int Mloc[OBJ_PROP_COUNT];

	obj_mtrl *mv;
	obj_vert *vv;
	obj_surf *sv;
} polyhedron;

void draw_polyhedron(polyhedron*);
polyhedron* obj_create(const char*);

#endif