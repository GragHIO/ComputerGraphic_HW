#include "HIO_include.h"



vector_3f::vector_3f()
{
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
}

vector_3f::vector_3f(int n)
{
	m_x = n;
	m_y = n;
	m_z = n;
}

vector_3f::vector_3f(float x, float y, float z):m_x(x), m_y(y), m_z(z)
{
}


vector_3f vector_3f::operator+(vector_3f right)
{	
	return vector_3f(m_x + right.m_x, m_y + right.m_y, m_z + right.m_z);
}

vector_3f vector_3f::operator-(vector_3f right)
{
	return vector_3f(m_x - right.m_x, m_y - right.m_y, m_z - right.m_z);
}

vector_3f vector_3f::operator*(double right)
{
	return vector_3f(m_x * right, m_y * right, m_z * right);
}

bool vector_3f::operator<(vector_3f const & right) const
{	
	return (this->length() < right.length());
}
bool vector_3f::operator>(vector_3f const & right) const
{
	return (this->length() > right.length());
}
const float vector_3f::operator[](const int idx) const
{	
	if (idx == 0) return m_x;
	if (idx == 1) return m_y;
	if (idx == 2) return m_z;	
	return 0.0f;
}
float vector_3f::length()const
{
	return sqrt(m_x*m_x + m_y*m_y + m_z * m_z);
}

vector_3f & vector_3f::operator+=(vector_3f right)
{
	this->m_x += right.m_x;
	this->m_y += right.m_y;
	this->m_z += right.m_z;
	return *this;	
}

vector_3f vector_3f::operator~()
{
	float length = sqrt(m_x*m_x + m_y*m_y + m_z*m_z);

	return vector_3f(m_x / length, m_y / length, m_z / length);
}

vector_3f vector_3f::operator-()
{
	return vector_3f(-m_x, -m_y, -m_z);
}

vector_3f::~vector_3f()
{
}
