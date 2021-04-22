#pragma once
class AABB
{
public:
	AABB();
	AABB(const  vector_3f &center, const vector_3f &halfwidths);
	bool AABB::operator &&(AABB b);
	~AABB();

	vector_3f c;        // center point
	vector_3f r;        // halfwidths
};

