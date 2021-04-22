#pragma once

class billboard :
	public obj
{
public:
	billboard();
	billboard(float ww, float hh);
	~billboard();
	void draw();
	vector_3f get_a();

	static vector_3f b;
	vector_3f P;
	vector_3f c;
	float w;
	float h;
};

