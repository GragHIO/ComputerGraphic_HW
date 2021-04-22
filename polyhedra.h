#pragma once

class polyhedra :
	public obj
{
public:
	polyhedra();
	polyhedra(char *);
	~polyhedra();
	void draw();
	polyhedron *uds;
};

