#pragma once

class group :
	public obj
{
public:
	group();
	group(std::string str);
	group(std::string str, vector_3f co);
	group(std::string str, vector_3f co, vector_4f ma);
	group* setTexture(std::vector<std::string> fileName, std::vector<bool> alphaZero);
	void draw();
	~group();

	std::string name;
	vector_3f color;
	vector_4f material;
	GLuint texture[32];
	size_t texture_cont;
};

