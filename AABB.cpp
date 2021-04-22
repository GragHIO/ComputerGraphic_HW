#include "HIO_include.h"

AABB::AABB()
{
}

AABB::AABB(const vector_3f & center, const vector_3f & halfwidths): c(center), r(halfwidths)
{
}

bool AABB::operator&&(AABB b)
{
	//printf("x: %f, %f\n", fabs(this->c[0] - b.c[0]), (this->r[0] + b.r[0]));
	//printf("y: %f, %f\n", fabs(this->c[1] - b.c[1]), (this->r[1] + b.r[1]));
	//printf("z: %f, %f\n", fabs(this->c[2] - b.c[2]), (this->r[2] + b.r[2]));

	if (fabs(this->c[0] - b.c[0]) > (this->r[0] + b.r[0])) return false;
	if (fabs(this->c[1] - b.c[1]) > (this->r[1] + b.r[1])) return false;
	if (fabs(this->c[2] - b.c[2]) > (this->r[2] + b.r[2])) return false;
	return true;
}


AABB::~AABB()
{
}
