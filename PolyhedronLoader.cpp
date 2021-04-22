#define _CRT_SECURE_NO_WARNINGS
#include "HIO_include.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>


#include <GL/glew.h>
#include <GL/glut.h>
#include "Polyhedron.h"
#include "PolyhedronLoader.h"
#include "ImageLoader.h"

#include <vector>

extern GLuint textureEmpty = 0;

typedef struct TEXTURE_MAP_ITEM {
	GLuint id;
	char* filename;
} texture_map_item;
std::vector<texture_map_item> texturemap = *(new std::vector<texture_map_item>());

void draw_polyhedron(polyhedron *O)
{
	if (O->vao == 0)
	{
		for (int si = 0; si < O->sc; ++si)
		{
			if (O->sv[si].pc > 0)
			{
				obj_surf surf = O->sv[si];
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, (O->mv[surf.mi].kv)[0].map);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, textureEmpty);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				for (int pi = 0; pi < surf.pc; ++pi)
				{
					glBegin(GL_POLYGON);
						glTexCoord2f(1 - O->vv[surf.pv[pi].vi[0]].t[0], 1 - O->vv[surf.pv[pi].vi[0]].t[1]);
						glNormal3fv(O->vv[surf.pv[pi].vi[0]].n);
						glVertex3fv(O->vv[surf.pv[pi].vi[0]].v);

						glTexCoord2f(1 - O->vv[surf.pv[pi].vi[1]].t[0], 1 - O->vv[surf.pv[pi].vi[1]].t[1]);
						glNormal3fv(O->vv[surf.pv[pi].vi[1]].n);
						glVertex3fv(O->vv[surf.pv[pi].vi[1]].v);

						glTexCoord2f(1 - O->vv[surf.pv[pi].vi[2]].t[0], 1 - O->vv[surf.pv[pi].vi[2]].t[1]);
						glNormal3fv(O->vv[surf.pv[pi].vi[2]].n);
						glVertex3fv(O->vv[surf.pv[pi].vi[2]].v);
					glEnd();
				}
			}
		}
	}
}

polyhedron *obj_create(const char *filename)
{
	polyhedron *O;
	int  i;

	/* Allocate and initialize a new file. */

	if ((O = (polyhedron *)calloc(1, sizeof(polyhedron))))
	{
		if (filename)
		{
			/* Read the named file. */

			read_obj(O, filename);

			/* Post-process the loaded object. */

			obj_mini(O);
			obj_proc(O);
		}

		/* Set default shader locations. */

		for (i = 0; i < OBJ_PROP_COUNT; i++)
		{
			O->cloc[i] = 0;
			O->oloc[i] = 0;
			O->Mloc[i] = 0;
		}
		O->uloc = 0;
		O->nloc = 0;
		O->tloc = 0;
		O->vloc = 0;
	}
	return O;
}

void obj_delete(polyhedron *O)
{
	assert(O);

	obj_rel(O);

	free(O);
}

static void read_obj(polyhedron *O, const char *filename)
{
	char buf[MAXSTR];
	char key[MAXSTR];

	char L[MAXSTR];
	char D[MAXSTR];

	FILE *fin;

	/* Flush the vector caches. */

	_vc = 0;
	_tc = 0;
	_nc = 0;
	_ic = 0;

	/* Add the named file to the given object. */

	if ((fin = fopen(filename, "r")))
	{
		/* Ensure there exists a default surface 0 and default material 0. */

		int si = obj_add_surf(O);
		int mi = obj_add_mtrl(O);
		int gi = 0;
		int n;

		obj_set_surf(O, si, mi);

		/* Extract the directory from the filename for use in MTL loading. */

		strncpy(D, filename, MAXSTR);
		dirpath(D);

		/* Process each line of the OBJ file, invoking the handler for each. */

		while (fgets(buf, MAXSTR, fin))
			if (sscanf(buf, "%s%n", key, &n) >= 1)
			{
				const char *c = buf + n;

				if (!strcmp(key, "f")) read_f(c, O, si, gi);
				else if (!strcmp(key, "l")) read_l(c, O, si);
				else if (!strcmp(key, "vt")) read_vt(c);
				else if (!strcmp(key, "vn")) read_vn(c);
				else if (!strcmp(key, "v")) read_v(c);

				else if (!strcmp(key, "mtllib"))      read_mtllib(L, c);
				else if (!strcmp(key, "usemtl")) si = read_usemtl(D, L, c, O);
				else if (!strcmp(key, "s")) gi = atoi(c);
			}

		fclose(fin);
	}
}

void obj_mini(polyhedron *O)
{
	int si;
	int mi;

	/* Remove empty surfaces. */

	for (si = O->sc - 1; si >= 0; --si)
		if (O->sv[si].pc == 0 &&
			O->sv[si].lc == 0)
			obj_del_surf(O, si);

	/* Remove unreferenced materials. */

	for (mi = O->mc - 1; mi >= 0; --mi)
	{
		int cc = 0;

		for (si = 0; si < O->sc; ++si)
			if (O->sv[si].mi == mi)
				cc++;

		if (cc == 0)
			obj_del_mtrl(O, mi);
	}
}


void obj_proc(polyhedron *O)
{
	int si;
	int sj;
	int pi;
	int vi;

	assert(O);

	/* Normalize all normals. Zero all tangent vectors. */

	for (vi = 0; vi < O->vc; ++vi)
	{
		normalize(O->vv[vi].n);

		O->vv[vi].u[0] = 0.0f;
		O->vv[vi].u[1] = 0.0f;
		O->vv[vi].u[2] = 0.0f;
	}

	/* Compute tangent vectors for all vertices. */

	for (si = 0; si < O->sc; ++si)
		for (pi = 0; pi < O->sv[si].pc; ++pi)
		{
			obj_vert *v0 = O->vv + O->sv[si].pv[pi].vi[0];
			obj_vert *v1 = O->vv + O->sv[si].pv[pi].vi[1];
			obj_vert *v2 = O->vv + O->sv[si].pv[pi].vi[2];

			float dt1, dv1[3];
			float dt2, dv2[3];

			float u[3];

			/* Compute the tangent vector for this polygon. */

			dv1[0] = v1->v[0] - v0->v[0];
			dv1[1] = v1->v[1] - v0->v[1];
			dv1[2] = v1->v[2] - v0->v[2];

			dv2[0] = v2->v[0] - v0->v[0];
			dv2[1] = v2->v[1] - v0->v[1];
			dv2[2] = v2->v[2] - v0->v[2];

			dt1 = v1->t[1] - v0->t[1];
			dt2 = v2->t[1] - v0->t[1];

			u[0] = dt2 * dv1[0] - dt1 * dv2[0];
			u[1] = dt2 * dv1[1] - dt1 * dv2[1];
			u[2] = dt2 * dv1[2] - dt1 * dv2[2];

			normalize(u);

			/* Accumulate the tangent vectors for this polygon's vertices. */

			v0->u[0] += u[0];  v0->u[1] += u[1];  v0->u[2] += u[2];
			v1->u[0] += u[0];  v1->u[1] += u[1];  v1->u[2] += u[2];
			v2->u[0] += u[0];  v2->u[1] += u[1];  v2->u[2] += u[2];
		}

	/* Orthonormalize each tangent basis. */

	for (vi = 0; vi < O->vc; ++vi)
	{
		float *n = O->vv[vi].n;
		float *u = O->vv[vi].u;

		float v[3];

		cross(v, n, u);
		cross(u, v, n);
		normalize(u);
	}

	/* Sort surfaces such that transparent ones appear later. */

	for (si = 0; si < O->sc; ++si)
		for (sj = si + 1; sj < O->sc; ++sj)
			if (O->mv[O->sv[si].mi].kv[OBJ_KD].c[3] <
				O->mv[O->sv[sj].mi].kv[OBJ_KD].c[3])
			{
				obj_surf temp;

				temp = O->sv[si];
				O->sv[si] = O->sv[sj];
				O->sv[sj] = temp;
			}
}

static void dirpath(char *pathname)
{
	int i;

	/* Find the path by cutting a file name at the last directory delimiter. */

	for (i = (int)strlen(pathname) - 1; i >= 0; --i)
		if (pathname[i] == '/' || pathname[i] == '\\')
		{
			pathname[i] = '\0';
			return;
		}

	/* If no delimiter was found, return the current directory. */

	strcpy(pathname, ".");
}

static void read_f(const char *line, polyhedron *O, int si, int gi)
{
	float n[3];
	float t[3];
	int i, pi;

	/* Create new vertex references for this face. */

	int i0 = _ic;
	int ic = read_poly_vertices(line, O, gi);

	/* If smoothing, apply this face's normal to vertices that need it. */

	if (gi)
	{
		normal(n, _vv[_iv[i0 + 0]._vi].v,
			_vv[_iv[i0 + 1]._vi].v,
			_vv[_iv[i0 + 2]._vi].v);

		for (i = 0; i < ic; ++i)
			if (_iv[i0 + 0]._ni < 0)
			{
				obj_get_vert_n(O, _iv[i0 + i]._vi, t);
				t[0] += n[0];
				t[1] += n[1];
				t[2] += n[2];
				obj_set_vert_n(O, _iv[i0 + i]._vi, t);
			}
	}

	/* Convert our N new vertex references into N-2 new triangles. */

	for (i = 0; i < ic - 2; ++i)

		if ((pi = obj_add_poly(O, si)) >= 0)
		{
			int vi[3];

			vi[0] = _iv[i0].vi;
			vi[1] = _iv[i0 + i + 1].vi;
			vi[2] = _iv[i0 + i + 2].vi;

			obj_set_poly(O, si, pi, vi);
		}
}

static void read_l(const char *line, polyhedron *O, int si)
{
	int i, li;

	/* Create new vertices for this line. */

	int i0 = _ic;
	int ic = read_line_vertices(line, O);

	/* Convert our N new vertices into N-1 new lines. */

	for (i = 0; i < ic - 1; ++i)

		if ((li = obj_add_line(O, si)) >= 0)
		{
			int vi[2];

			vi[0] = _iv[i0 + i].vi;
			vi[1] = _iv[i0 + i + 1].vi;

			obj_set_line(O, si, li, vi);
		}
}


static void read_v(const char *line)
{
	int _vi;

	/* Parse a vertex position. */

	if ((_vi = add_v()) >= 0)
	{
		sscanf(line, "%f %f %f", _vv[_vi].v + 0,
			_vv[_vi].v + 1,
			_vv[_vi].v + 2);
		_vv[_vi]._ii = -1;
	}
}

static void read_vt(const char *line)
{
	int _ti;

	/* Parse a texture coordinate. */

	if ((_ti = add_t()) >= 0)
	{
		sscanf(line, "%f %f", _tv[_ti].v + 0,
			_tv[_ti].v + 1);
		_tv[_ti]._ii = -1;
	}
}

static void read_vn(const char *line)
{
	int _ni;

	/* Parse a normal. */

	if ((_ni = add_n()) >= 0)
	{
		sscanf(line, "%f %f %f", _nv[_ni].v + 0,
			_nv[_ni].v + 1,
			_nv[_ni].v + 2);
		_nv[_ni]._ii = -1;
	}
}

static void read_image(polyhedron *O, int mi, int ki, const char *line,
	const char *path)
{
	unsigned int clamp = 0;

	float o[3] = { 0.0f, 0.0f, 0.0f };
	float s[3] = { 1.0f, 1.0f, 1.0f };

	char pathname[MAXSTR];

	char map[MAXSTR];
	char val[MAXSTR];

	const char *end;

	memset(map, 0, MAXSTR);
	memset(val, 0, MAXSTR);

	while (line[0] != '\0' && line[0] != '\r' && line[0] != '\n')
	{
		int n = 0;

		while (isspace(line[0])) line++;

		/* Parse property map options. */

		if (sscanf(line, "-clamp %s%n", val, &n) >= 1)
		{
			clamp = (strcmp(val, "on") == 0) ? OBJ_OPT_CLAMP : 0;
			line += n;
		}

		/* Parse property map scale. */

		else if (sscanf(line, "-s %f %f %f%n", s + 0, s + 1, s + 2, &n) >= 3)
			line += n;
		else if (sscanf(line, "-s %f %f%n", s + 0, s + 1, &n) >= 2)
			line += n;
		else if (sscanf(line, "-s %f%n", s + 0, &n) >= 1)
			line += n;

		/* Parse property map offset. */

		else if (sscanf(line, "-o %f %f %f%n", o + 0, o + 1, o + 2, &n) >= 3)
			line += n;
		else if (sscanf(line, "-o %f %f%n", o + 0, o + 1, &n) >= 2)
			line += n;
		else if (sscanf(line, "-o %f%n", o + 0, &n) >= 1)
			line += n;

		/* Check for a file name */

		else if ((end = strstr(line, ".tga"))) { strncpy(map, line, end - line + 4); break; }
		else if ((end = strstr(line, ".bmp"))) { strncpy(map, line, end - line + 4); break; }
		else if ((end = strstr(line, ".BMP"))) { strncpy(map, line, end - line + 4); break; }
		else if ((end = strstr(line, ".TGA"))) { strncpy(map, line, end - line + 4); break; }

		/* If we see something we don't recognize, stop looking. */

		else break;
	}

	/* Apply all parsed property attributes to the material. */

	sprintf(pathname, "%s/%s", path, map);

	obj_set_mtrl_opt(O, mi, ki, clamp);
	obj_set_mtrl_map(O, mi, ki, pathname);
	obj_set_mtrl_o(O, mi, ki, o);
	obj_set_mtrl_s(O, mi, ki, s);
}

static void read_color(polyhedron *O, int mi, int ki, const char *line)
{
	float c[4];

	/* Merge incoming color components with existing defaults. */

	obj_get_mtrl_c(O, mi, ki, c);
	sscanf(line, "%f %f %f", c, c + 1, c + 2);
	obj_set_mtrl_c(O, mi, ki, c);
}

static void read_alpha(polyhedron *O, int mi, int ki, const char *line)
{
	float c[4];
	float t;

	/* Merge incoming color components with existing defaults. */

	obj_get_mtrl_c(O, mi, ki, c);
	sscanf(line, "%f", &t);
	c[3] = 1.0 - t;
	obj_set_mtrl_c(O, mi, ki, c);
}

static void read_mtl(const char *path,
	const char *file,
	const char *name, polyhedron *O, int mi)
{
	char pathname[MAXSTR];

	char buf[MAXSTR];
	char key[MAXSTR];
	char arg[MAXSTR];

	FILE *fin;

	int scanning = 1;
	int n = 0;

	sprintf(pathname, "%s/%s", path, file);

	if ((fin = fopen(pathname, "r")))
	{
		/* Process each line of the MTL file. */

		while (fgets(buf, MAXSTR, fin))
			if (sscanf(buf, "%s%n", key, &n) >= 1)
			{
				const char *c = buf + n;

				if (scanning)
				{
					/* Determine if we've found the MTL we're looking for. */

					if (!strcmp(key, "newmtl"))
					{
						sscanf(c, "%s", arg);

						if ((scanning = strcmp(arg, name)) == 0)
							obj_set_mtrl_name(O, mi, name);
					}
				}
				else
				{
					/* Stop scanning when the next MTL begins. */

					if (!strcmp(key, "newmtl"))
						break;

					/* Parse this material's properties. */

					else if (!strcmp(key, "map_Kd"))
						read_image(O, mi, OBJ_KD, c, path);
					else if (!strcmp(key, "map_Ka"))
						read_image(O, mi, OBJ_KA, c, path);
					else if (!strcmp(key, "map_Ke"))
						read_image(O, mi, OBJ_KE, c, path);
					else if (!strcmp(key, "map_Ks"))
						read_image(O, mi, OBJ_KS, c, path);
					else if (!strcmp(key, "map_Ns"))
						read_image(O, mi, OBJ_NS, c, path);
					else if (!strcmp(key, "map_Kn"))
						read_image(O, mi, OBJ_KN, c, path);

					else if (!strcmp(key, "Kd"))
						read_color(O, mi, OBJ_KD, c);
					else if (!strcmp(key, "Ka"))
						read_color(O, mi, OBJ_KA, c);
					else if (!strcmp(key, "Ke"))
						read_color(O, mi, OBJ_KE, c);
					else if (!strcmp(key, "Ks"))
						read_color(O, mi, OBJ_KS, c);
					else if (!strcmp(key, "Ns"))
						read_color(O, mi, OBJ_NS, c);

					else if (!strcmp(key, "d"))
						read_alpha(O, mi, OBJ_KD, c);
					else if (!strcmp(key, "Tr"))
						read_alpha(O, mi, OBJ_KD, c);
				}
			}
		fclose(fin);
	}
}

static void read_mtllib(char *file, const char *line)
{
	/* Parse the first file name from the given line. */

	sscanf(line, "%s", file);
}

static int read_usemtl(const char *path,
	const char *file,
	const char *line, polyhedron *O)
{
	char name[MAXSTR];

	int si;
	int mi;

	sscanf(line, "%s", name);

	/* Create a new material for the incoming definition. */

	if ((mi = obj_add_mtrl(O)) >= 0)
	{
		/* Create a new surface to contain geometry with the new material. */

		if ((si = obj_add_surf(O)) >= 0)
		{
			/* Read the material definition and apply it to the new surface. */

			read_mtl(path, file, name, O, mi);
			obj_set_surf(O, si, mi);

			/* Return the surface so that new geometry may be added to it. */

			return si;
		}
	}

	/* On failure, return the default surface. */

	return 0;
}

static int add__(void **_v, int *_c, int *_m, size_t _s)
{
	int   m = (*_m > 0) ? *_m * 2 : 2;
	void *v;

	/* If space remains in the current block, return it. */

	if (*_m > *_c)
		return (*_c)++;

	/* Else, try to increase the size of the block. */

	else if ((v = realloc(*_v, _s * m)))
	{
		*_v = v;
		*_m = m;
		return (*_c)++;
	}

	/* Else, indicate failure. */

	else return -1;
}

static int add_v(void)
{
	return add__((void **)&_vv, &_vc, &_vm, sizeof(vec3));
}

static int add_t(void)
{
	return add__((void **)&_tv, &_tc, &_tm, sizeof(vec2));
}

static int add_n(void)
{
	return add__((void **)&_nv, &_nc, &_nm, sizeof(vec3));
}

static int add_i(void)
{
	return add__((void **)&_iv, &_ic, &_im, sizeof(iset));
}

static char *set_name(char *old, const char *src)
{
	char *dst = NULL;

	if (old)
		free(old);

	if (src && (dst = (char *)malloc(strlen(src) + 1)))
		strcpy(dst, src);

	return dst;
}

void obj_set_mtrl_name(polyhedron *O, int mi, const char *name)
{
	assert_mtrl(O, mi);
	O->mv[mi].name = set_name(O->mv[mi].name, name);
}

void obj_set_mtrl_map(polyhedron *O, int mi, int ki, const char *str)
{
	assert_prop(O, mi, ki);

#ifndef CONF_NO_GL
	if (O->mv[mi].kv[ki].map)
		glDeleteTextures(1, &O->mv[mi].kv[ki].map);
#endif

	O->mv[mi].kv[ki].map = obj_load_image(str);
	O->mv[mi].kv[ki].str = set_name(O->mv[mi].kv[ki].str, str);
}

void obj_set_mtrl_opt(polyhedron *O, int mi, int ki, unsigned int opt)
{
	assert_prop(O, mi, ki);

	O->mv[mi].kv[ki].opt = opt;
}

void obj_set_mtrl_c(polyhedron *O, int mi, int ki, const float c[4])
{
	assert_prop(O, mi, ki);

	O->mv[mi].kv[ki].c[0] = c[0];
	O->mv[mi].kv[ki].c[1] = c[1];
	O->mv[mi].kv[ki].c[2] = c[2];
	O->mv[mi].kv[ki].c[3] = c[3];
}

void obj_set_mtrl_s(polyhedron *O, int mi, int ki, const float s[3])
{
	assert_prop(O, mi, ki);

	O->mv[mi].kv[ki].s[0] = s[0];
	O->mv[mi].kv[ki].s[1] = s[1];
	O->mv[mi].kv[ki].s[2] = s[2];
}

void obj_set_mtrl_o(polyhedron *O, int mi, int ki, const float o[3])
{
	assert_prop(O, mi, ki);

	O->mv[mi].kv[ki].o[0] = o[0];
	O->mv[mi].kv[ki].o[1] = o[1];
	O->mv[mi].kv[ki].o[2] = o[2];
}


void obj_del_mtrl(polyhedron *O, int mi)
{
	int si;

	assert_mtrl(O, mi);

	/* Remove this material from the material vector. */

	obj_rel_mtrl(O->mv + mi);

	memmove(O->mv + mi,
		O->mv + mi + 1,
		(O->mc - mi - 1) * sizeof(obj_mtrl));

	O->mc--;

	/* Remove all references to this material. */

	for (si = O->sc - 1; si >= 0; --si)
	{
		obj_surf *sp = O->sv + si;

		if (sp->mi == mi)
			obj_del_surf(O, si);
		else
			if (sp->mi > mi)
				sp->mi--;
	}
}

void obj_del_vert(polyhedron *O, int vi)
{
	int si;
	int pi;
	int li;

	assert_vert(O, vi);

	/* Remove this vertex from the file's vertex vector. */

	memmove(O->vv + vi,
		O->vv + vi + 1,
		(O->vc - vi - 1) * sizeof(obj_vert));

	O->vc--;

	/* Remove all references to this vertex from all surfaces. */

	for (si = 0; si < O->sc; ++si)
	{
		/* Delete all referencing polygons. Decrement later references. */

		for (pi = O->sv[si].pc - 1; pi >= 0; --pi)
		{
			obj_poly *pp = O->sv[si].pv + pi;

			if (pp->vi[0] == vi || pp->vi[1] == vi || pp->vi[2] == vi)
				obj_del_poly(O, si, pi);
			else
			{
				if (pp->vi[0] > vi) pp->vi[0]--;
				if (pp->vi[1] > vi) pp->vi[1]--;
				if (pp->vi[2] > vi) pp->vi[2]--;
			}
		}

		/* Delete all referencing lines. Decrement later references. */

		for (li = O->sv[si].lc - 1; li >= 0; --li)
		{
			obj_line *lp = O->sv[si].lv + li;

			if (lp->vi[0] == vi || lp->vi[1] == vi)
				obj_del_line(O, si, li);
			else
			{
				if (lp->vi[0] > vi) lp->vi[0]--;
				if (lp->vi[1] > vi) lp->vi[1]--;
			}
		}
	}

	/* Schedule the VBO for refresh. */

	invalidate(O);
}

void obj_del_poly(polyhedron *O, int si, int pi)
{
	assert_poly(O, si, pi);

	/* Remove this polygon from the surface's polygon vector. */

	memmove(O->sv[si].pv + pi,
		O->sv[si].pv + pi + 1,
		(O->sv[si].pc - pi - 1) * sizeof(obj_poly));

	O->sv[si].pc--;
}

void obj_del_line(polyhedron *O, int si, int li)
{
	assert_line(O, si, li);

	/* Remove this line from the surface's line vector. */

	memmove(O->sv[si].lv + li,
		O->sv[si].lv + li + 1,
		(O->sv[si].lc - li - 1) * sizeof(obj_line));

	O->sv[si].lc--;
}

void obj_del_surf(polyhedron *O, int si)
{
	assert_surf(O, si);

	/* Remove this surface from the file's surface vector. */

	obj_rel_surf(O->sv + si);

	memmove(O->sv + si,
		O->sv + si + 1,
		(O->sc - si - 1) * sizeof(obj_surf));

	O->sc--;
}

static void cross(float *z, const float *x, const float *y)
{
	float t[3];

	t[0] = x[1] * y[2] - x[2] * y[1];
	t[1] = x[2] * y[0] - x[0] * y[2];
	t[2] = x[0] * y[1] - x[1] * y[0];

	z[0] = t[0];
	z[1] = t[1];
	z[2] = t[2];
}

static void normalize(float *v)
{
	float k = 1.0f / (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	v[0] *= k;
	v[1] *= k;
	v[2] *= k;
}

static void normal(float *n, const float *a,
	const float *b,
	const float *c)
{
	float u[3];
	float v[3];

	u[0] = b[0] - a[0];
	u[1] = b[1] - a[1];
	u[2] = b[2] - a[2];

	v[0] = c[0] - a[0];
	v[1] = c[1] - a[1];
	v[2] = c[2] - a[2];

	cross(n, u, v);
	normalize(n);
}

static void obj_rel_mtrl(obj_mtrl *mp)
{
	/* Release any resources held by this material. */

	int ki;

	for (ki = 0; ki < OBJ_PROP_COUNT; ki++)
	{
		if (mp->kv[ki].str) free(mp->kv[ki].str);
#ifndef CONF_NO_GL
		if (mp->kv[ki].map) glDeleteTextures(1, &mp->kv[ki].map);
#endif
	}
}

static void obj_rel_surf(obj_surf *sp)
{
	if (sp->pibo) glDeleteBuffers(1, &sp->pibo);
	if (sp->libo) glDeleteBuffers(1, &sp->libo);

	sp->pibo = 0;
	sp->libo = 0;

	/* Release this surface's polygon and line vectors. */

	if (sp->pv) free(sp->pv);
	if (sp->lv) free(sp->lv);
}

static void obj_rel(polyhedron *O)
{
	int si;
	int mi;

	if (O->vbo) glDeleteBuffers(1, &O->vbo);
	if (O->vao) glDeleteVertexArrays(1, &O->vao);

	O->vbo = 0;

	for (mi = 0; mi < O->mc; ++mi) obj_rel_mtrl(O->mv + mi);
	for (si = 0; si < O->sc; ++si) obj_rel_surf(O->sv + si);
}

static int read_poly_indices(const char *line, int *_vi, int *_ti, int *_ni)
{
	int n;

	*_vi = 0;
	*_ti = 0;
	*_ni = 0;

	/* Parse a face vertex specification from the given line. */

	if (sscanf(line, "%d/%d/%d%n", _vi, _ti, _ni, &n) >= 3) return n;
	if (sscanf(line, "%d/%d%n", _vi, _ti, &n) >= 2) return n;
	if (sscanf(line, "%d//%d%n", _vi, _ni, &n) >= 2) return n;
	if (sscanf(line, "%d%n", _vi, &n) >= 1) return n;

	return 0;
}

static int read_poly_vertices(const char *line, polyhedron *O, int gi)
{
	const char *c = line;

	int _vi;
	int _ti;
	int _ni;
	int _ii;
	int _ij;

	int  dc;
	int  vi;
	int  ic = 0;

	/* Scan the face string, converting index sets to vertices. */

	while ((dc = read_poly_indices(c, &_vi, &_ti, &_ni)))
	{
		/* Convert face indices to vector cache indices. */

		_vi += (_vi < 0) ? _vc : -1;
		_ti += (_ti < 0) ? _tc : -1;
		_ni += (_ni < 0) ? _nc : -1;

		/* Initialize a new index set. */

		if ((_ii = add_i()) >= 0)
		{
			_iv[_ii]._vi = _vi;
			_iv[_ii]._ni = _ni;
			_iv[_ii]._ti = _ti;

			/* Search the vector reference list for a repeated index set. */

			for (_ij = _vv[_vi]._ii; _ij >= 0; _ij = _iv[_ij]._ii)
				if (_iv[_ij]._vi == _vi &&
					_iv[_ij]._ti == _ti &&
					_iv[_ij]._ni == _ni &&
					_iv[_ij].gi == gi)
				{
					/* A repeat has been found. Link new to old. */

					_vv[_vi]._ii = _ii;
					_iv[_ii]._ii = _ij;
					_iv[_ii].vi = _iv[_ij].vi;
					_iv[_ii].gi = _iv[_ij].gi;

					break;
				}

			/* If no repeat was found, add a new vertex. */

			if ((_ij < 0) && (vi = obj_add_vert(O)) >= 0)
			{
				_vv[_vi]._ii = _ii;
				_iv[_ii]._ii = -1;
				_iv[_ii].vi = vi;
				_iv[_ii].gi = gi;

				/* Initialize the new vertex using valid cache references. */

				if (0 <= _vi && _vi < _vc) obj_set_vert_v(O, vi, _vv[_vi].v);
				if (0 <= _ni && _ni < _nc) obj_set_vert_n(O, vi, _nv[_ni].v);
				if (0 <= _ti && _ti < _tc) obj_set_vert_t(O, vi, _tv[_ti].v);
			}
			ic++;
		}
		c += dc;
	}
	return ic;
}

void obj_get_vert_v(const polyhedron *O, int vi, float *v)
{
	assert_vert(O, vi);

	v[0] = O->vv[vi].v[0];
	v[1] = O->vv[vi].v[1];
	v[2] = O->vv[vi].v[2];
}

void obj_get_vert_t(const polyhedron *O, int vi, float *t)
{
	assert_vert(O, vi);

	t[0] = O->vv[vi].t[0];
	t[1] = O->vv[vi].t[1];
}

void obj_get_vert_n(const polyhedron *O, int vi, float *n)
{
	assert_vert(O, vi);

	n[0] = O->vv[vi].n[0];
	n[1] = O->vv[vi].n[1];
	n[2] = O->vv[vi].n[2];
}

static void invalidate(polyhedron *O)
{
	if (O->vbo) glDeleteBuffers(1, &O->vbo);
	if (O->vao) glDeleteVertexArrays(1, &O->vao);
	O->vbo = 0;
	O->vao = 0;
}

void obj_set_vert_v(polyhedron *O, int vi, const float v[3])
{
	assert_vert(O, vi);

	O->vv[vi].v[0] = v[0];
	O->vv[vi].v[1] = v[1];
	O->vv[vi].v[2] = v[2];

	invalidate(O);
}

void obj_set_vert_t(polyhedron *O, int vi, const float t[2])
{
	assert_vert(O, vi);

	O->vv[vi].t[0] = t[0];
	O->vv[vi].t[1] = t[1];

	invalidate(O);
}

void obj_set_vert_n(polyhedron *O, int vi, const float n[3])
{
	assert_vert(O, vi);

	O->vv[vi].n[0] = n[0];
	O->vv[vi].n[1] = n[1];
	O->vv[vi].n[2] = n[2];

	invalidate(O);
}

void obj_set_vert_u(polyhedron *O, int vi, const float u[3])
{
	assert_vert(O, vi);

	O->vv[vi].u[0] = u[0];
	O->vv[vi].u[1] = u[1];
	O->vv[vi].u[2] = u[2];

	invalidate(O);
}

int obj_add_mtrl(polyhedron *O)
{
	unsigned int opt = 0;

	const float Kd[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	const float Ka[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	const float Ke[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float Ks[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float Ns[4] = { 8.0f, 0.0f, 0.0f, 0.0f };
	const float  s[3] = { 1.0f, 1.0f, 1.0f };

	int mi;

	assert(O);

	/* Allocate and initialize a new material. */

	if ((mi = add__((void **)&O->mv,
		&O->mc,
		&O->mm, sizeof(obj_mtrl))) >= 0)
	{
		memset(O->mv + mi, 0, sizeof(obj_mtrl));

		obj_set_mtrl_opt(O, mi, OBJ_KD, opt);
		obj_set_mtrl_opt(O, mi, OBJ_KA, opt);
		obj_set_mtrl_opt(O, mi, OBJ_KE, opt);
		obj_set_mtrl_opt(O, mi, OBJ_KS, opt);
		obj_set_mtrl_opt(O, mi, OBJ_NS, opt);
		obj_set_mtrl_opt(O, mi, OBJ_KN, opt);

		obj_set_mtrl_c(O, mi, OBJ_KD, Kd);
		obj_set_mtrl_c(O, mi, OBJ_KA, Ka);
		obj_set_mtrl_c(O, mi, OBJ_KE, Ke);
		obj_set_mtrl_c(O, mi, OBJ_KS, Ks);
		obj_set_mtrl_c(O, mi, OBJ_NS, Ns);

		obj_set_mtrl_s(O, mi, OBJ_KD, s);
		obj_set_mtrl_s(O, mi, OBJ_KA, s);
		obj_set_mtrl_s(O, mi, OBJ_KE, s);
		obj_set_mtrl_s(O, mi, OBJ_KS, s);
		obj_set_mtrl_s(O, mi, OBJ_NS, s);
		obj_set_mtrl_s(O, mi, OBJ_KN, s);
	}
	return mi;
}

int obj_add_vert(polyhedron *O)
{
	int vi;

	assert(O);

	/* Allocate and initialize a new vertex. */

	if ((vi = add__((void **)&O->vv,
		&O->vc,
		&O->vm, sizeof(obj_vert))) >= 0)

		memset(O->vv + vi, 0, sizeof(obj_vert));

	return vi;
}

int obj_add_poly(polyhedron *O, int si)
{
	int pi;

	assert_surf(O, si);

	/* Allocate and initialize a new polygon. */

	if ((pi = add__((void **)&O->sv[si].pv,
		&O->sv[si].pc,
		&O->sv[si].pm, sizeof(obj_poly))) >= 0)

		memset(O->sv[si].pv + pi, 0, sizeof(obj_poly));

	return pi;
}

int obj_add_line(polyhedron *O, int si)
{
	int li;

	assert_surf(O, si);

	/* Allocate and initialize a new line. */

	if ((li = add__((void **)&O->sv[si].lv,
		&O->sv[si].lc,
		&O->sv[si].lm, sizeof(obj_line))) >= 0)

		memset(O->sv[si].lv + li, 0, sizeof(obj_line));

	return li;
}

int obj_add_surf(polyhedron *O)
{
	int si;

	assert(O);

	/* Allocate and initialize a new surface. */

	if ((si = add__((void **)&O->sv,
		&O->sc,
		&O->sm, sizeof(obj_surf))) >= 0)

		memset(O->sv + si, 0, sizeof(obj_surf));

	return si;
}


void obj_set_poly(polyhedron *O, int si, int pi, const int vi[3])
{
	assert_poly(O, si, pi);

	O->sv[si].pv[pi].vi[0] = (index_t)vi[0];
	O->sv[si].pv[pi].vi[1] = (index_t)vi[1];
	O->sv[si].pv[pi].vi[2] = (index_t)vi[2];
}

void obj_set_line(polyhedron *O, int si, int li, const int vi[2])
{
	assert_line(O, si, li);

	O->sv[si].lv[li].vi[0] = (index_t)vi[0];
	O->sv[si].lv[li].vi[1] = (index_t)vi[1];
}

void obj_set_surf(polyhedron *O, int si, int mi)
{
	assert_surf(O, si);

	O->sv[si].mi = mi;
}

static int read_line_indices(const char *line, int *_vi, int *_ti)
{
	int n;

	*_vi = 0;
	*_ti = 0;

	/* Parse a line vertex specification from the given line. */

	if (sscanf(line, "%d/%d%n", _vi, _ti, &n) >= 2) return n;
	if (sscanf(line, "%d%n", _vi, &n) >= 1) return n;

	return 0;
}

static int read_line_vertices(const char *line, polyhedron *O)
{
	const char *c = line;

	int _vi;
	int _ti;
	int _ii;
	int _ij;

	int  dc;
	int  vi;
	int  ic = 0;

	/* Scan the line string, converting index sets to vertices. */

	while ((dc = read_line_indices(c, &_vi, &_ti)))
	{
		/* Convert line indices to vector cache indices. */

		_vi += (_vi < 0) ? _vc : -1;
		_ti += (_ti < 0) ? _tc : -1;

		/* Initialize a new index set. */

		if ((_ii = add_i()) >= 0)
		{
			_iv[_ii]._vi = _vi;
			_iv[_ii]._ti = _ti;

			/* Search the vector reference list for a repeated index set. */

			for (_ij = _vv[_vi]._ii; _ij >= 0; _ij = _iv[_ij]._ii)
				if (_iv[_ij]._vi == _vi &&
					_iv[_ij]._ti == _ti)
				{
					/* A repeat has been found. Link new to old. */

					_vv[_vi]._ii = _ii;
					_iv[_ii]._ii = _ij;
					_iv[_ii].vi = _iv[_ij].vi;

					break;
				}

			/* If no repeat was found, add a new vertex. */

			if ((_ij < 0) && (vi = obj_add_vert(O)) >= 0)
			{
				_vv[_vi]._ii = _ii;
				_iv[_ii]._ii = -1;
				_iv[_ii].vi = vi;

				/* Initialize the new vertex using valid cache references. */

				if (0 <= _vi && _vi < _vc) obj_set_vert_v(O, vi, _vv[_vi].v);
				if (0 <= _ti && _ti < _tc) obj_set_vert_t(O, vi, _tv[_ti].v);
			}
			ic++;
		}
		c += dc;
	}
	return ic;
}

const char *obj_get_mtrl_name(const polyhedron *O, int mi)
{
	assert_mtrl(O, mi);
	return O->mv[mi].name;
}

unsigned int obj_get_mtrl_map(const polyhedron *O, int mi, int ki)
{
	assert_prop(O, mi, ki);
	return O->mv[mi].kv[ki].map;
}

unsigned int obj_get_mtrl_opt(const polyhedron *O, int mi, int ki)
{
	assert_prop(O, mi, ki);
	return O->mv[mi].kv[ki].opt;
}

void obj_get_mtrl_c(const polyhedron *O, int mi, int ki, float *c)
{
	assert_prop(O, mi, ki);

	c[0] = O->mv[mi].kv[ki].c[0];
	c[1] = O->mv[mi].kv[ki].c[1];
	c[2] = O->mv[mi].kv[ki].c[2];
	c[3] = O->mv[mi].kv[ki].c[3];
}

void obj_get_mtrl_s(const polyhedron *O, int mi, int ki, float *s)
{
	assert_prop(O, mi, ki);

	s[0] = O->mv[mi].kv[ki].s[0];
	s[1] = O->mv[mi].kv[ki].s[1];
	s[2] = O->mv[mi].kv[ki].s[2];
}

void obj_get_mtrl_o(const polyhedron*O, int mi, int ki, float *o)
{
	assert_prop(O, mi, ki);

	o[0] = O->mv[mi].kv[ki].o[0];
	o[1] = O->mv[mi].kv[ki].o[1];
	o[2] = O->mv[mi].kv[ki].o[2];
}

unsigned int obj_load_image(const char *filename)
{
	unsigned int o = 0;
	if (filename)
	{
		
		if (endswith(filename, "bmp")) {
			int texturemap_size = texturemap.size();
			for (int i = 0; i < texturemap_size; i++) {
				if (!strcmp(filename, texturemap[i].filename)) {
					return texturemap[i].id;
				}
			}
			o = LoadTextureBMP(filename);
			char* tmp_name = new char[strlen(filename) + 2];
			strcpy(tmp_name, filename);
			texture_map_item temp_texture_data = { o, tmp_name };
			texturemap.push_back(temp_texture_data);
			return o;
		}
	}
	return o;
}

int endswith(const char* haystack, const char* needle)
{
	size_t hlen;
	size_t nlen;
	/* find the length of both arguments -
	if needle is longer than haystack, haystack can't end with needle */
	hlen = strlen(haystack);
	nlen = strlen(needle);
	if (nlen > hlen) return 0;

	/* see if the end of haystack equals needle */
	return (strcmp(&haystack[hlen - nlen], needle)) == 0;
}

unsigned int LoadTextureBMP(const char * filename, bool alphaZero)
{
	unsigned int texture;

	int width, height;

	unsigned char * data = NULL;
	auto test = loadded_texture->find(const_cast<char*>(filename));
	
	if (test != loadded_texture->end())return test->second;

	printf("loding texture file %s ...\n", filename);
	readBMPfile(const_cast<char*>(filename), &width, &height, &data);;//data
	if (data == NULL) return 0;
	if (alphaZero)
	{
		unsigned char * dataAarr = new unsigned char[width * height * 4];
		unsigned char * t;
		for (int i = 0; i < width * height; ++i)
		{
			if (data[i * 3] == 255 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
			{
				dataAarr[i * 4] = dataAarr[i * 4 + 1] = dataAarr[i * 4 + 2] = dataAarr[i * 4 + 3] = 0;
			}
			else
			{
				dataAarr[i * 4] = data[i * 3];
				dataAarr[i * 4 + 1] = data[i * 3 + 1];
				dataAarr[i * 4 + 2] = data[i * 3 + 2];
				dataAarr[i * 4 + 3] = 255;
			}
		}
		t = dataAarr;
		dataAarr = data;
		data = t;
		delete[] dataAarr;
	}
	

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, (alphaZero) ? GL_RGBA : GL_RGB, width, height, 0,
		(alphaZero) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	gluBuild2DMipmaps(GL_TEXTURE_2D, (alphaZero) ? 4 : 3, width, height, (alphaZero) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	(alphaZero) ? delete[]data : free(data);

	if (test == loadded_texture->end()) 
		(*loadded_texture)[filename] = texture;

	return texture;
}