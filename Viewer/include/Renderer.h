#pragma once
#include "Scene.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

/*
 * Renderer class.
 */
class Renderer
{
private:
	float *colorBuffer;
	float *zBuffer;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;

	void putPixel(int x, int y, const glm::vec3& color);
	void createBuffers(int viewportWidth, int viewportHeight);

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);

	void drawTraingle(float x1, float y1, float x2, float y2, float x3, float y3 , const glm::vec3& color) ;
	void PrintLineBresenham(int x1, int y1, int x2, int y2, const glm::vec3& color, int toFlip = 0, int fllag = 0);
	void PrintLineBresenham(const glm::vec3& point1, const glm::vec3& point2);
	void PrintLine(const glm::vec3& point, const glm::vec3& direction,glm::mat4 transform);
	void UpdateWorldTransform(const Scene& scene) const;
	//TODO
	/*glm::mat4& ModelFrameTransform() const;*/
	void UpdateViewTransform(const Scene& scene) const;
	//void PrintActiveModelFaceNormals(const Scene& scene)const;
	void PrintNormalPerFace(std::shared_ptr<const MeshModel> model, glm::mat4 matrix) ;
	void PrintBoundingBox(std::shared_ptr<const MeshModel>model, glm::mat4 matrix);
	void PrintNormalPerVertix(std::shared_ptr<const MeshModel> model, glm::mat4 matrix);
	glm::mat4 GetViewPortTramsform()const;
};