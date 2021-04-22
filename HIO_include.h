#pragma once
#define PI 3.141592653589793
#define radius(x) (x * PI / 180.0)

#include <stdio.h>
#include <string>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <map>
using namespace std;
//#pragma comment(lib, "glew32.lib")
#include <GL/glew.h>
#include <GL/glut.h>
#include "Polyhedron.h"
#include "linked_list.h"
#include "list_tree.h"


#include "vector_3f.h"
#include "vector_4f.h"
#include "base.h"
#include "obj.h"
#include "trans.h"

#include "ball.h"
#include "cube.h"
#include "prism.h"
#include "group.h"
#include "polyhedra.h"

#include "AABB.h"
#include "entity.h"
#include "robot.h"
#include "ground_floor.h"
#include "obstacle.h"
#include "billboard.h"
#include "fog.h"

#include "light.h"
#include "pointolite.h"
#include "directional_light.h"
#include "Spot_light.h"
#include "globalVar.h"
#include "main.h"

#include "ImageLoader.h"
