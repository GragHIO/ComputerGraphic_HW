#include "HIO_include.h"



obstacle::obstacle()
{
	can_move = false;
}

obstacle::obstacle(AABB box)
{
	bounding_box = box;
	//coord->m_shift = box.c;
	can_move = false;
}



void obstacle::bounce(vector_3f S)
{
	if (can_move) 
	{
		coord->m_shift += S;	
		bounding_box.c += S;
	}
}


obstacle::~obstacle()
{
}

vector_3f obstacle::get_location()
{
	return bounding_box.c;
}

void obstacle::update(double t)
{
}

void obstacle::set_coord(trans * the_coord)
{
	coord = the_coord;
	bounding_box.c = the_coord->m_shift;
}
