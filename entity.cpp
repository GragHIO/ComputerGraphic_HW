#include "HIO_include.h"



entity::entity()
{
}


entity::~entity()
{
}

void entity::update_entity(double t)
{
	update(t);
	bounding_box.c = get_location();
}
