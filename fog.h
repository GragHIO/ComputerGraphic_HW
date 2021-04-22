#pragma once
class fog
{
public:
	fog();
	~fog();
	static vector_4f color_arr[3];
	vector_4f color;
	float density;
	int type;
	float start, end;
};

