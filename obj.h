#pragma once

class obj :
	public base
{
public:
	obj();
	~obj();
	virtual void draw() = 0;
	void draw_light();
};

