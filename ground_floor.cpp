#include "HIO_include.h"



ground_floor::ground_floor()
{
	this->bounding_box = { {0,-1,0}, {25,1,25} };
	can_move = false;
}


ground_floor::~ground_floor()
{
}

void ground_floor::bounce(vector_3f S)
{
}

void ground_floor::update(double t)
{

}

vector_3f ground_floor::get_location()
{
	return vector_3f(0, -1, 0);
}
