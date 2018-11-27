#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include "ImguiMenus.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)



static void printS(glm::vec3 vector) {
	std::cout << "vector before:" << std::endl;
	std::cout << "x " << vector.x << " y " << vector.y << " z " << vector.z << std::endl;
}

static void printSS(glm::vec3 vector) {
	std::cout << "vector after:" << std::endl;
	std::cout << "x " << vector.x << " y " << vector.y << " z " << vector.z << std::endl;
}

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

	colorBuffer = new float[3* viewportWidth * viewportHeight];
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
	//PrintLineBresenham(200,500,300,100,glm::vec3(0,0,0));
	//Draw_Line_Bresenham(200, 500, 300, 100, glm::vec3(0, 0, 0));
	if (scene.GetModelCount() <= 0) return;
	std::shared_ptr<const MeshModel> activeModel = scene.GetAciveModel();
	static int _r = 0;
	Camera activeCamera = scene.GetActiveCamera();
	Renderer::UpdateWorldTransform(scene);
	glm::mat4 worldT = activeModel->GetWorldTransformation();
	glm::mat4 IvT = activeCamera.GetInverseViewTranform();
	glm::mat4 pT = activeCamera.GetProjectionTransform();
	std::vector<glm::vec3> vertix;
	for (int i = 0; i < activeModel->GetVerticesCount(); i++) {
		vertix = activeModel->GetVertices(i);
		float x[3], y[3];
		glm::vec4 transformedV[3];
		if (vertix.size() == 3) {
			for (int k = 0; k < 3; k++){
				transformedV[k] = /*glm::transpose*Utils::Translate(glm::vec3(300, 300, 300)))*/glm::transpose(pT)*glm::transpose(IvT)*glm::transpose(Utils::Scale(glm::vec3(20000, 20000, 20000)))*glm::transpose(worldT)*Utils::HomCoordinats(vertix.at(k));
				//transformedV[k] = glm::transpose(worldT*Utils::Scale(glm::vec3(20000, 20000, 20000))*IvT*pT*Utils::Translate(glm::vec3(300, 300, 300)))*Utils::HomCoordinats(vertix.at(k));
				x[k] = transformedV[k].x;
				y[k] = transformedV[k].y;
			}
			if (_r++ <= 0) {
				std::cout << "x " << x[0] << " y " << y[0] << std::endl;
			}
			drawTraingle(x[0], y[0], x[1], y[1], x[2], y[2], GetMeshColor());
		}
		vertix.clear();
	}
}




void Renderer::PrintLineBresenham(int x1, int y1, int x2, int y2, const glm::vec3& Color,int toFlip,int fllag) 
{
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

	for (int x = (int)x1; x < maxX; x++)
	{
		if (steep)
		{
			putPixel(y, x, Color);
		}
		else
		{
			putPixel(x, y, Color);
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
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
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

void Renderer::drawTraingle(float x1, float y1, float x2, float y2, float x3, float y3, const glm::vec3& color)  {
	Renderer::PrintLineBresenham(x1, y1, x2, y2, color);
	Renderer::PrintLineBresenham(x2, y2, x3, y3,color);
	Renderer::PrintLineBresenham(x3, y3, x1, y1, color);
}

void Renderer::UpdateWorldTransform(const Scene& scene) const {
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	glm::mat4 modelTransform= activeModel->GetWorldTransformation();
	static float thetaX= GetXAxisRotation();
	static float thetaY = GetYAxisRotation();
	static float thetaZ = GetZAxisRotation();
	if (thetaX != GetXAxisRotation()) {
		float dif=GetXAxisRotation()-thetaX;
		activeModel->SetWorldTransformation(Utils::RotateOrigin(dif,X)*activeModel->GetWorldTransformation());
		thetaX += dif;
	}
	else if (thetaY!=GetYAxisRotation()) {
		float dif = GetYAxisRotation()-thetaY;
		activeModel->SetWorldTransformation(Utils::RotateOrigin(dif, Y)*activeModel->GetWorldTransformation());
		thetaY += dif;
	}
	else if(thetaZ!=GetZAxisRotation()){
		float dif=GetZAxisRotation()- thetaZ;
		activeModel->SetWorldTransformation(Utils::RotateOrigin(dif, Z)*activeModel->GetWorldTransformation());
		thetaZ += dif;
	}
}

void Renderer::UpdateViewTransform(const Scene& scene) const {
	//TODO
}