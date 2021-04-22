#pragma once
class entity
{
public:
	entity();
	~entity();
	virtual void update(double t) = 0;
	virtual vector_3f get_location() = 0;
	virtual void bounce(vector_3f S) = 0;
	void update_entity(double t);

	AABB bounding_box;

	bool can_move;
};

