#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef POLYHEDRONLOADER_H
#define POLYHEDRONLOADER_H

#define MAXSTR 1024
#define OBJ_OPT_CLAMP  1

#define assert_surf(O, i) \
      { assert(O); assert(0 <= i && i < O->sc); }
#define assert_vert(O, i) \
      { assert(O); assert(0 <= i && i < O->vc); }
#define assert_mtrl(O, i) \
      { assert(O); assert(0 <= i && i < O->mc); }

#define assert_line(O, i, j) \
      { assert_surf(O, i); assert(0 <= j && j < O->sv[i].lc); }
#define assert_poly(O, i, j) \
      { assert_surf(O, i); assert(0 <= j && j < O->sv[i].pc); }
#define assert_prop(O, i, j) \
      { assert_mtrl(O, i); assert(0 <= j && j < polyhedron_data_t::OBJ_PROP_COUNT); }

static int _vc, _vm;
static int _tc, _tm;
static int _nc, _nm;
static int _ic, _im;

typedef struct VEC_2
{
	float v[2];
	int _ii;
} vec2;

typedef struct VEC_3
{
	float v[3];
	int _ii;
} vec3;

typedef struct VEC_4
{
	float v[4];
	int _ii;
} vec4;

typedef struct I_SET
{
	int vi;
	int gi;

	int _vi;
	int _ti;
	int _ni;
	int _ii;
} iset;

static vec3 *_vv;
static vec2 *_tv;
static vec3 *_nv;
static iset *_iv;

static void read_obj(polyhedron*, const char*);
static void obj_rel(polyhedron*);
unsigned int obj_load_image(const char*);

static void read_l(const char*, polyhedron*, int);
static void read_v(const char *);
static void read_f(const char*, polyhedron*, int, int);
static void read_vt(const char *);
static void read_vn(const char *);
static void read_image(polyhedron*, int, int, const char*, const char*);
static void read_color(polyhedron*, int, int, const char*);
static void read_alpha(polyhedron*, int, int, const char *);
static void read_mtl(const char*, const char*, const char*, polyhedron*, int);
static void read_mtllib(char*, const char*);
static int read_poly_indices(const char*, int*, int*, int*);
static int read_poly_vertices(const char*, polyhedron*, int);
static int read_line_indices(const char*, int*, int*);
static int read_line_vertices(const char*, polyhedron*);
static int read_usemtl(const char*, const char*, const char*, polyhedron*);

static int add__(void**, int*, int*, size_t);
static int add_v(void);
static int add_t(void);
static int add_n(void);
static int add_i(void);

static void normalize(float*);
static void cross(float*, const float*, const float*);
static void normal(float*, const float*, const float*, const float*);
static void invalidate(polyhedron*);
int endswith(const char*, const char*);

void obj_del_mtrl(polyhedron*, int);
void obj_del_surf(polyhedron*, int);
void obj_del_vert(polyhedron*, int);
void obj_del_poly(polyhedron*, int, int);
void obj_del_line(polyhedron*, int, int);

static void obj_rel(polyhedron*);
static void obj_rel_mtrl(obj_mtrl*);
static void obj_rel_surf(obj_surf*);

int obj_add_surf(polyhedron*);
int obj_add_mtrl(polyhedron*);
int obj_add_vert(polyhedron*);
int obj_add_poly(polyhedron*, int);
int obj_add_line(polyhedron*, int);

void obj_mini(polyhedron*);
void obj_proc(polyhedron*);

void obj_set_surf(polyhedron*, int, int);

void obj_set_mtrl_name(polyhedron*, int, const char*);
void obj_set_mtrl_map(polyhedron*, int, int, const char*);
void obj_set_mtrl_opt(polyhedron*, int, int, unsigned int);
void obj_set_mtrl_c(polyhedron*, int, int, const float[4]);
void obj_set_mtrl_s(polyhedron*, int, int, const float[3]);
void obj_set_mtrl_o(polyhedron*, int, int, const float[3]);

void obj_set_vert_v(polyhedron*, int, const float[3]);
void obj_set_vert_t(polyhedron*, int, const float[2]);
void obj_set_vert_n(polyhedron*, int, const float[3]);
void obj_set_vert_u(polyhedron*, int, const float[3]);

void obj_set_poly(polyhedron*, int, int, const int[3]);
void obj_set_line(polyhedron*, int, int, const int[2]);
void obj_set_surf(polyhedron*, int, int);

void obj_get_vert_v(const polyhedron*, int, float*);
void obj_get_vert_t(const polyhedron*, int, float*);
void obj_get_vert_n(const polyhedron*, int, float*);

const char* obj_get_mtrl_name(const polyhedron*, int);
unsigned int obj_get_mtrl_map(const polyhedron*, int, int);
unsigned int obj_get_mtrl_opt(const polyhedron*, int, int);
void obj_get_mtrl_c(const polyhedron*, int, int, float*);
void obj_get_mtrl_s(const polyhedron*, int, int, float*);
void obj_get_mtrl_o(const polyhedron*, int, int, float*);

static void dirpath(char*);
#endif