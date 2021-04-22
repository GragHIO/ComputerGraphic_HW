#include "HIO_include.h"

group::group()
{
	texture_cont = 0;
}

group::group(std::string str)
{
	name = str;
	material = { 0.3f,0.9f,0.8f,32.0f };
	texture_cont = 0;
}

group::group(std::string str, vector_3f co)
{
	name = str;
	color = co;
	material = { 0.3f,0.9f,0.8f,32.0f };
	texture_cont = 0;
}

group::group(std::string str, vector_3f co, vector_4f ma)
{
	name = str;
	color = co;
	material = ma;
	texture_cont = 0;
}

group* group::setTexture(std::vector<std::string> fileName, std::vector<bool> alphaZero)
{
	
	for (int i = 0; i < fileName.size(); i++)
		texture[i] = LoadTextureBMP(fileName[i].c_str(), alphaZero[i]);

	texture_cont = fileName.size();
	return this;
}



void group::draw()
{
	if (texture > 0) glEnable(GL_TEXTURE_2D);
	else glDisable(GL_TEXTURE_2D);

	glMaterialfv(GL_FRONT, GL_AMBIENT,  vector_4f(color * material[0], 1));
	if(material[3] >= 9486)glMaterialfv(GL_FRONT, GL_EMISSION, vector_4f(vector_3f(0, 0.3, 0), 1));
	else glMaterialfv(GL_FRONT, GL_EMISSION, vector_4f(vector_3f(0, 0, 0), 1));
	if(material[1] > 1)
		glMaterialfv(GL_FRONT, GL_SPECULAR, vector_4f(vector_3f(1,1,1), 1));
	else
		glMaterialfv(GL_FRONT, GL_SPECULAR, vector_4f(color * material[1], 1));
	glMaterialfv(GL_FRONT, GL_DIFFUSE,  vector_4f(color * material[2], 1));
	glMaterialf(GL_FRONT, GL_SHININESS, material[3]);
	glColor3f(color.m_x, color.m_y, color.m_z);

	for (int i = 0; i < 32; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + (size_t)i);
		if(i >= texture_cont) glBindTexture(GL_TEXTURE_2D, GL_ZERO);
		else
		{			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
		}
	}
	glActiveTexture(GL_TEXTURE0);

}


group::~group()
{
}
