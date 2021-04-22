#pragma once
class vector_4f
{
public:
	vector_4f();
	vector_4f(int n);
	vector_4f(float x, float y, float z, float w = 1);
	vector_4f(vector_3f vec, float w = 1);
	operator float *();
	~vector_4f();

	float m_x;
	float m_y;
	float m_z;
	float m_w;
};

