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
	glm::mat4 ViewPortTransform;
	void putPixel(int x, int y, const glm::vec3& color);
	void putZ(int x, int y,  float z);
	float getZ(int i, int j);
	void createBuffers(int viewportWidth, int viewportHeight);

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();
	void UpdateWorldTransform(const Scene& scene) const;
	void PrintNormalPerFace(std::shared_ptr<const MeshModel> model, glm::mat4 matrix);
	void PrintBoundingBox(std::shared_ptr<const MeshModel>model, glm::mat4 matrix);
	void PrintNormalPerVertix(std::shared_ptr<const MeshModel> model, glm::mat4 matrix);
	void PrintLineBresenham(int x1, int y1,float z1, int x2, int y2,float z2, const glm::vec3& color, int toFlip = 0, int fllag = 0);
	void PrintLineBresenham(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& color=glm::vec3(0,0,0));
	//void PrintLine(const glm::vec3& point, const glm::vec3& direction, glm::mat4 transform);
	void PrintLine(const glm::vec3& point, const glm::vec3& direction, glm::mat4 transform,const glm::vec3& color=glm::vec3(0,0,0));
	void UpdateModelRotation(const Scene& scene)const;
	void UpdateModelScale(const Scene& scene)const;
	void UpdateModelTranslate(const Scene& scene)const;
	void UpdateWorldTranslate(const Scene& scene)const;
	void UpdateWorldRotation(const Scene& scene)const;
	void UpdateModelColor(const Scene& scene)const;
	void PrintFrame(const float length,const glm::mat4 matrix);
	void PrintWorldFrame(const Scene& scene);
	void PrintModelFrame(const Scene& scene);
	void PrintModel(const Scene& scene,std::shared_ptr<const MeshModel> model,glm::mat4 matrix1/*, glm::mat4 matrix2*/,const glm::vec3& col);
	void PrintAllModels(const Scene& scene);
	void PrintAllCameras(const Scene& scene);
	void PrintTraingle(const Scene& scene,const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& col);
	void PrintTraingle(const Scene&scene,const std::vector<glm::vec3>& vertix, glm::mat4 transform/*, glm::mat4 transform2*/, const glm::vec3& col);
	glm::vec3 calculateColor(const Scene& scene,const glm::vec3& position,const glm::vec3& materialColor)const;
public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	glm::mat4 GetViewPortTramsform();
};