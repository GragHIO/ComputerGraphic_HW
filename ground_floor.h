#pragma once
#include "entity.h"
class ground_floor :
	public entity
{
public:
	ground_floor();
	~ground_floor();
	void bounce(vector_3f S);
	void update(double t);
	vector_3f get_location();
};

