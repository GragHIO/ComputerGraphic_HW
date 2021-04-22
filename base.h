#pragma once
class base
{
public:
	base();
	~base();
	virtual void draw() = 0;
	virtual void draw_light() = 0;
};

