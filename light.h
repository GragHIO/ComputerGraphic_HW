#pragma once
class light:
	public obj
{
protected:
	virtual void init();
	virtual void turnOn() = 0;
	virtual void turnOff() = 0;
public:
	light();
	light(int id);
	~light();

	GLenum get_light_id();

	void open();
	void close();
	void draw_light();
	virtual void draw() = 0;

	vector_4f diffuse; // light color
	vector_4f specular;// ®nµ€øO¨›™∫√C¶ÅE

	const GLenum light_id;
	bool light_state;
};

