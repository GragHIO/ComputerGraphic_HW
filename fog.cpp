#include "HIO_include.h"

vector_4f fog::color_arr[3] = {
	{ 0.7f, 0.7f, 0.7f, 1.0 },{ 0.2f, 0.9f, 0.9f, 1.0f },{ 0.9f, 0.2f, 0.2f, 1.0f }

};

fog::fog()
{
	color = color_arr[0];
	density = 0.2;
	type = 0; // off
	start = 0;
	end = 10;
}


fog::~fog()
{
}
