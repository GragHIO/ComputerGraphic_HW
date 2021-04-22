#pragma once

class cube :
	public obj
{
public:
	cube();
	~cube();
	void draw();
	void cross(int i);
	static float  points[][3];
	static float  text_coor[4][2];
	static int    face[][4];

	vector_3f     n;
};

