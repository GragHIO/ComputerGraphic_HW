#include "HIO_include.h"



vector_4f::vector_4f():m_x(0), m_y(0), m_z(0), m_w(0)
{
}

vector_4f::vector_4f(int n):m_x(n), m_y(n), m_z(n), m_w(n)
{

}

vector_4f::vector_4f(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w)
{
}

vector_4f::vector_4f(vector_3f vec, float w) : m_x(vec.m_x), m_y(vec.m_y), m_z(vec.m_z), m_w(w)
{
}

vector_4f::operator float*()
{
	return (float *)(this);
}



vector_4f::~vector_4f()
{
}
