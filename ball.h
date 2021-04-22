#pragma once

class ball :
	public obj
{
public:
	ball();
	~ball();
	void draw();
	static GLUquadricObj  *sphere;

};

