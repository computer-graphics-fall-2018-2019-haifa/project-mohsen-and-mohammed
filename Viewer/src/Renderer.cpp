#define _USE_MATH_DEFINES
#include <iostream>
#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr)
{
	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
};

void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
	//colorBuffer[INDEX(viewportWidth, i, j, 0)] = 0.0f;
	//colorBuffer[INDEX(viewportWidth, i, j, 1)] = 0.0f;
	//colorBuffer[INDEX(viewportWidth, i, j, 2)] = 0.0f;
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	colorBuffer = new float[3 * viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			putPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			putPixel(i, j, color);
		}
	}
}

void Renderer::SetViewport(int viewportWidth, int viewportHeight, int viewportX, int viewportY)
{
	this->viewportX = viewportX;
	this->viewportY = viewportY;
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	createBuffers(viewportWidth, viewportHeight);
	createOpenGLBuffer();
}

void Renderer::Render(const Scene& scene)
{
	if (scene.GetModelCount() > 0)
	{
		const MeshModel& myMishModel = scene.GetModel(scene.GetActiveModelIndex());

		for (int i = 0; i < myMishModel.GetFacesSize(); i++)
		{
			const glm::vec3 vec1 = myMishModel.GetVertices(i, 0);
			const glm::vec3 vec2 = myMishModel.GetVertices(i, 1);
			const glm::vec3 vec3 = myMishModel.GetVertices(i, 2);

			glm::vec4 nvec1(vec1.x, vec1.y, vec1.z, 1);

			glm::vec4 nvec2(vec2.x, vec2.y, vec2.z, 1);

			glm::vec4 nvec3(vec3.x, vec3.y, vec3.z, 1);


			glm::mat4x4 mat44 = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0, };

			/*
			mat44=scene.GetActiveCameraIndex()

			*/

			glm::vec4 vec11 = mat44*nvec1;
			glm::vec4 vec22 = mat44*nvec2;
			glm::vec4 vec33 = mat44*nvec3;

			glm::vec3 color = glm::vec3(0, 0, 0);

			Line((vec11.x + 1)*100+200, (vec11.y + 1) * 100 + 200, (vec22.x + 1) * 100 + 200, (vec22.y + 1) * 100 + 200, color);
			Line((vec22.x + 1) *100+200, (vec22.y + 1) * 100 + 200, (vec33.x + 1) * 100 + 200, (vec33.y + 1) * 100 + 200, color);
			Line((vec33.x + 1) * 100+200, (vec33.y + 1) * 100 + 200, (vec11.x + 1) * 100 + 200, (vec11.y + 1) * 100 + 200, color);

			//PrintLineBresenham(499, 498, 497, 499, color);
			//std::cout << "x=" << (vec11.x + 5) * 100 << " y=" << (vec22.x + 5) * 100 << " z=" << (vec11.y + 5) * 100  << "lrb3 " << (vec22.y + 5) * 100 << "\n";
			//PrintLineBresenham((vec11.x + 1) * 100, (vec11.y + 1) * 100, (vec22.x + 1) * 100, (vec22.y + 1) * 100, color);
			//PrintLineBresenham((vec22.x + 51)*100, (vec22.y+51) * 100, (vec33.x+51) * 100, (vec33.y+51) * 100, color);
			//PrintLineBresenham((vec33.x + 51) * 100, (vec33.y + 51) * 100, (vec11.x + 51) * 100, (vec11.y + 51) * 100, color);


		}

	}

	//#############################################
	//## You should override this implementation ##
	//## Here you should render the scene.       ##
	//#############################################

	// Draw a chess board in the middle of the screen

	PrintLineBresenham(0, 0, 300, 100, glm::vec3(1, 0, 0));

	/*
	for (int i = 100; i < viewportWidth - 100; i++)
	{
	for (int j = 100; j < viewportHeight - 100; j++)
	{
	int mod_i = i / 2;
	int mod_j = j / 2;

	int odd = (mod_i + mod_j) % 2;
	if (odd)
	{
	putPixel(i, j, glm::vec3(1, 1, 0));
	}
	else
	{
	putPixel(i, j, glm::vec3(1, 0, 0));
	}
	}
	}*/

}





void Renderer::PrintLineBresenham(int x1, int y1, int x2, int y2, const glm::vec3& color, int toFlip)
{
	/*the equation of the line wich passes through (x1,y1) and (x2,y2) is
	y=slope*x + distance*//*,distance=y1-slope*x1*/
	float slope = 0.0;
	if (x2 != x1) slope = (float)(y2 - y1) / (float)(x2 - x1);
	int flag = (slope >= 0) ? 1 : -1/*,toFlip=0*/;
	if (fabs(slope) > 1 || (x1 == x2)) {
		PrintLineBresenham(y1, x1, y2, x2, color, 1);
		return;
	}
	int x = 0, y = 0, xMax = 0, dx = 0, dy = 0, error = 0;
	if (x1 <= x2) {
		x = x1; y = y1; xMax = x2; dx = x2 - x1; dy = y2 - y1;
	}
	else {
		x = x2; y = y2; xMax = x1; dx = x1 - x2; dy = y1 - y2; ;
	}
	error = -dx;
	while (x < xMax) {
		if (flag*error > 0) {
			y += flag; error -= 2 * dx;
		}
		if (toFlip) {
			putPixel(y, x, color);
		}
		else {
			putPixel(x, y, color);
		}
		x++; error += 2 * dy;
	}
}





void Renderer::Line(float x1, float y1,  float x2,  float y2, glm::vec3& Color)
{
	// Bresenham's line algorithm
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for (int x = (int)x1; x<maxX; x++)
	{
		if (steep)
		{
			putPixel(y, x, Color);
			//SetPixel(y, x, color);
		}
		else
		{
			putPixel(x, y, Color);
			//SetPixel(x, y, color);
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}






//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		1, -1,
		-1,  1,
		-1,  1,
		1, -1,
		1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewportWidth, viewportHeight, GL_RGB, GL_FLOAT, colorBuffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}