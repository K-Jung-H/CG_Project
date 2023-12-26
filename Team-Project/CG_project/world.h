#pragma once
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

struct Field
{
	GLfloat vertex[4][3] =
	{
		{-60.0f, 0.0f, -60.0f},
		{-60.0f, 0.0f, 60.0f},
		{60.0f, 0.0f, 60.0f},
		{60.0f, 0.0f, -60.0f}
	};
	GLfloat color[4][3] =
	{
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f}
	};
	GLfloat normal[4][3] =
	{
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f}
	};
	GLuint I[6]
	{
		//¹Ù´Ú
		2, 1, 0,
		3, 2, 0
	};


	GLuint vao;
	GLuint vbo_v; // vertex
	GLuint vbo_c; // color
	GLuint vbo_n; // normal
	GLuint ebo;
};
