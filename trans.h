#pragma once

class trans :
	public base
{
public:
	trans();
	trans(vector_3f angle, vector_3f scale, vector_3f shift);
	~trans();
	void draw();
	void draw_light();
	
	vector_3f m_angle;
	vector_3f m_scale;
	vector_3f m_shift;
};

