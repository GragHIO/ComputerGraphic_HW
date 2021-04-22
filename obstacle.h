#pragma once

class obstacle :
	public entity
{
public:
	obstacle();
	obstacle(AABB box);
	void bounce(vector_3f S);
	~obstacle();
	
	vector_3f get_location();
	void update(double t);
	void set_coord(trans* the_coord);

	trans *coord;
};

