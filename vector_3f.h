#pragma once
class vector_3f
{
public:
	vector_3f();
	vector_3f(int n);
	vector_3f(float x, float y, float z);	
	vector_3f operator +(vector_3f right);
	vector_3f operator -(vector_3f right);
	vector_3f operator *(double right);
	bool operator <(vector_3f const& right)const;
	bool operator >(vector_3f const& right)const;
	const float operator[](const int idx) const;
	float length() const;
	vector_3f& operator +=(vector_3f right);
	vector_3f operator ~();		//norm
	vector_3f operator -();
	~vector_3f();

	float m_x;
	float m_y;
	float m_z;
};

