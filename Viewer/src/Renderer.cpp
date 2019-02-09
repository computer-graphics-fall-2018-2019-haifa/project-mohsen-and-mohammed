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
#define ZINDEX(width,x,y) ((x)+(y)*(width))
#define DEGREETORADIAN(theta)  (2 * theta / 360.0f*M_PI)
#define WORLDFRAMEAXIS 30.0f
#define MODELFRAMAXIS 15.0f
#define MAX(x,y) (x>=y)?x:y
#define MIN(x,y) (x<y)?x:y
//static bool *tempBuffer=nullptr;
static float maxFloat = std::numeric_limits<float>::max();
static float max(float a, float b, float c) {
	if (a >= b && a >= c) return a;
	else if (b >= a && b >= c) return b;
	else return c;
}
static float min(float a, float b, float c) {
	if (a <= b && a <= c) return a;
	else if (b <= a && b <= c) return b;
	else return c;
}
static void printq(const glm::vec3& v) {
	std::cout << v.x << " " << v.y << " " << v.z << " " << "SDF" << std::endl;
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
	if (zBuffer) {
		delete[] zBuffer;
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

void Renderer::putZ(int i, int j, float z) {
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	zBuffer[ZINDEX(viewportWidth, i, j)] = z;
}

float Renderer::getZ(int i, int j) {
	return zBuffer[ZINDEX(viewportWidth, i, j)];
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
	if (zBuffer) {
		delete[] zBuffer;
	}
	colorBuffer = new float[3* viewportWidth * viewportHeight];
	zBuffer = new float[viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			putPixel(x, y, glm::vec3(0.8f, 0.8f, 0.8f));
			putZ(x,y, maxFloat);
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
			putZ(i, j, maxFloat);
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
	glm::mat4 scaleM = Utils::Scale(glm::vec3(viewportWidth / 2, viewportHeight / 2, 1));
	glm::mat4 translateM = Utils::Translate(glm::vec3(viewportWidth / 2, viewportHeight / 2, 0));
	this->ViewPortTransform = scaleM * translateM;
	createOpenGLBuffer();
}

void Renderer::Render(const Scene& scene)
{
	//print world frame
	Renderer::PrintWorldFrame(scene);
	if (PrintAllCameras_()) {
		Renderer::PrintAllCameras(scene);
	}
	if (scene.GetModelCount() <= 0) return;
	std::shared_ptr< MeshModel> activeModel = scene.GetAciveModel();
	Camera activeCamera = scene.GetActiveCamera();
	Renderer::UpdateWorldTransform(scene);
	glm::mat4 modelT = activeModel->GetModelTransformation();
	glm::mat4 worldT = activeModel->GetWorldTransformation();
	glm::mat4 IvT = activeCamera.GetInverseViewTranform();
	glm::mat4 pT = activeCamera.GetProjectionTransform();
	glm::mat4 VpT = Renderer::GetViewPortTramsform();
	std::vector<glm::vec3> vertix;
	glm::mat4 finalM = glm::transpose(VpT)*glm::transpose(pT)*glm::transpose(IvT)*glm::transpose(worldT)*glm::transpose(modelT);
	Renderer::PrintWorldFrame(scene);
	//print model frame
	Renderer::PrintModelFrame(scene);
	//print model
	glm::vec4 col = activeModel->GetColor();
	//glm::mat4 finalMt = glm::transpose(pT)*glm::transpose(IvT)*glm::transpose(worldT)*glm::transpose(modelT);
	Renderer::PrintModel(scene,activeModel, finalM,glm::vec3(col.x,col.y,col.z));
	//print normal per face
	if (DrawFaceNormal()) {
		Renderer::PrintNormalPerFace(activeModel, finalM);
	}
	//print bounding box
	if (DrawBoundingBox()) {
		Renderer::PrintBoundingBox(activeModel, finalM);
	}
	//print normal per vertix
	if (DrawVertixNormal()) {
		Renderer::PrintNormalPerVertix(activeModel, finalM);
	}
	if (PrintAllModels_()) {
		Renderer::PrintAllModels(scene);
	}
}

void Renderer::PrintLineBresenham(int x1, int y1,float z1, int x2, int y2,float z2, const glm::vec3& Color,int toFlip,int fllag) 
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
		std::swap(z1, z2);
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
		//	if (zBuffer[ZINDEX(viewportHeight, y, x)] > Utils::ZInterpolation(y, x, glm::vec3(x1, y1, z1), glm::vec3(x2, y2, z2))) {
			//	putZ(y, x, Utils::ZInterpolation(y, x, glm::vec3(x1, y1, z1), glm::vec3(x2, y2, z2)));
				putPixel(y, x, Color);
			//}
			
		}
		else
		{
			//if (zBuffer[ZINDEX(viewportHeight, x, y)] > Utils::ZInterpolation(x, y, glm::vec3(x1, y1, z1), glm::vec3(x2, y2, z2))) {
				//putZ(x, y, Utils::ZInterpolation(x, y, glm::vec3(x1, y1, z1), glm::vec3(x2, y2, z2)));
				putPixel(x, y, Color);
			//}
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

void Renderer::UpdateWorldTransform(const Scene& scene) const {
	Renderer::UpdateModelTranslate(scene);
	Renderer::UpdateWorldTranslate(scene);
	Renderer::UpdateModelRotation(scene);
	Renderer::UpdateModelScale(scene);
	Renderer::UpdateWorldRotation(scene);
	Renderer::UpdateModelColor(scene);
}

void Renderer::PrintNormalPerFace(std::shared_ptr<const MeshModel> model, glm::mat4 mat) {
	if (model->GetFaceCount() <= 0) return;
	for (int i = 0; i < model->GetFaceCount();i++) {
		glm::vec3 triangleCenter = model->GetFaceCenter(i);
		glm::vec3 normalDirection=model->GetFaceNormalDirection(i);
		Renderer::PrintLine(triangleCenter, normalDirection,mat);
	}
}

void Renderer::PrintBoundingBox(std::shared_ptr<const MeshModel>model, glm::mat4 matrix) {
	glm::vec3 up1, up2, up3, up4, down1, down2, down3, down4;
	const float minX=model->getMinX()
			  , maxX=model->getMaxX()
			  , minY=model->getMinY()
		      , maxY=model->getMaxY()
		      , minZ=model->getMinZ()
		      , maxZ=model->getMaxZ();
	up1 = glm::vec3(minX, maxY, maxZ);
	up2 = glm::vec3(maxX,maxY,maxZ);
	up3 = glm::vec3(maxX, minY, maxZ);
	up4 = glm::vec3(minX, minY, maxZ);
	down1 = glm::vec3(minX, maxY, minZ);
	down2 = glm::vec3(maxX, maxY, minZ);
	down3 = glm::vec3(maxX, minY, minZ);
	down4 = glm::vec3(minX, minY, minZ);
	Renderer::PrintLine(up1, up2 - up1, matrix);
	Renderer::PrintLine(up1, up4 - up1, matrix);
	Renderer::PrintLine(up3, up2 - up3, matrix);
	Renderer::PrintLine(up3, up4 - up3, matrix);
	Renderer::PrintLine(down1, down2 - down1,matrix);
	Renderer::PrintLine(down1, down4 - down1, matrix);
	Renderer::PrintLine(down3, down2 - down3, matrix);
	Renderer::PrintLine(down3,down4-down3,matrix);
	Renderer::PrintLine(up1,down1-up1 , matrix);
	Renderer::PrintLine(up2, down2 - up2, matrix);
	Renderer::PrintLine(up3, down3 - up3, matrix);
	Renderer::PrintLine(up4, down4 - up4, matrix);
}

void Renderer::PrintNormalPerVertix(std::shared_ptr<const MeshModel> model, glm::mat4 matrix)  {
	if (model == nullptr) return;
	for (int i = 0; i < model->GetVerticesCount();i++) {
		PrintLine(model->getVertix(i),model->getVertixNormal(i),matrix);
	}
}

glm::mat4 Renderer::GetViewPortTramsform() {
	//glm::mat4 scaleM = Utils::Scale(glm::vec3(viewportWidth / 2, viewportHeight / 2, 1));
	//glm::mat4 translateM = Utils::Translate(glm::vec3(viewportWidth / 2, viewportHeight / 2, 0));
	//this->ViewPortTransform = scaleM * translateM;
	return this->ViewPortTransform;
	//glm::mat4 scaleM = Utils::Scale(glm::vec3(viewportWidth/2, viewportHeight/2,1));
	//glm::mat4 translateM = Utils::Translate(glm::vec3(viewportWidth / 2, viewportHeight / 2, 0));
	//return  scaleM*translateM;
}

void Renderer::PrintLineBresenham(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& color) {
	Renderer::PrintLineBresenham(point1.x,point1.y,point1.z,point2.x,point2.y,point2.z, color);
}

void Renderer::PrintLine(const glm::vec3& point, const glm::vec3& direction, glm::mat4 transform,const glm::vec3& color) {
	const glm::vec3 temp1 = Utils::SwitchFromHom((transform)*Utils::HomCoordinats(point));
	const glm::vec3 temp2 = Utils::SwitchFromHom((transform)*Utils::HomCoordinats(point+direction));
	Renderer::PrintLineBresenham(temp1, temp2,color);
}

void Renderer::UpdateModelScale(const Scene& scene)const {
	if (GetScale().x == 0 || GetScale().y == 0 || GetScale().z == 0) return;
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	float difX= GetScale().x/activeModel->getModelScale().x;
	float difY= GetScale().y/ activeModel->getModelScale().y;
	float difZ= GetScale().z/ activeModel->getModelScale().z;
	activeModel->SetModelTransformation(activeModel->GetModelTransformation()*Utils::Scale(glm::vec3(difX, difY, difZ)));
	activeModel->setModelScale(GetScale());
}

void Renderer::PrintFrame(const float length, const glm::mat4 matrix) {
	const glm::vec3 Origin(0, 0, 0);
	Renderer::PrintLine(Origin, glm::vec3(length, 0, 0), matrix,glm::vec3(1,0,0));
	Renderer::PrintLine(Origin, glm::vec3(0, length, 0), matrix,glm::vec3(0,1,0));
	Renderer::PrintLine(Origin, glm::vec3(0, 0, length), matrix,glm::vec3(0,0,1));
}

void Renderer::PrintWorldFrame(const Scene& scene) {
	if (scene.GetCameraCount() <= 0) return;
	Camera activeCamera = scene.GetActiveCamera();
	//activeCamera.SetCameraLookAt(GetEye(), GetAt(), GetY());
	glm::mat4 IvT = activeCamera.GetInverseViewTranform();
	glm::mat4 pT = activeCamera.GetProjectionTransform();
	glm::mat4 VpT = Renderer::GetViewPortTramsform();
	Renderer::PrintFrame(WORLDFRAMEAXIS, glm::transpose(VpT)*glm::transpose(pT)*glm::transpose(IvT));
}

void Renderer::PrintModelFrame(const Scene& scene) {
	glm::vec3 scale = GetScale();
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	Camera activeCamera = scene.GetActiveCamera();
	//activeCamera.SetCameraLookAt(GetEye(), GetAt(), GetY());
	glm::mat4 wT = activeModel->GetWorldTransformation();
	glm::mat4 IvT = activeCamera.GetInverseViewTranform();
	glm::mat4 pT = activeCamera.GetProjectionTransform();
	glm::mat4 VpT = Renderer::GetViewPortTramsform();
	Renderer::PrintFrame(MODELFRAMAXIS, glm::transpose(VpT)*glm::transpose(pT)*glm::transpose(IvT)*glm::transpose(wT));
}

void Renderer::UpdateModelTranslate(const Scene& scene)const {
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	const glm::vec3 previousTranslate = activeModel->getModelTranslate();
	const glm::vec3 newTranslate = GetWorldTranslate();
	if (previousTranslate==newTranslate) return;
	const float difX = newTranslate.x- previousTranslate.x ;
	const float difY = newTranslate.y-previousTranslate.y;
	const float difZ = newTranslate.z-previousTranslate.z;
	activeModel->SetModelTransformation(activeModel->GetModelTransformation()*(Utils::Translate(glm::vec3(difX, difY, difZ))));
	activeModel->setModelTranslate(newTranslate);
}

void Renderer::UpdateWorldTranslate(const Scene& scene)const {
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	const glm::vec3 translateVector = activeModel->getWorldTranslate();
	const glm::vec3 newTranslateVector = GetWorldTranslateVector();
	const glm::vec3 dif = newTranslateVector - translateVector;
	activeModel->SetWorldTransformation(activeModel->GetWorldTransformation()*Utils::Translate(dif));
	activeModel->setWorldTranslate(newTranslateVector);
}

void Renderer::UpdateWorldRotation(const Scene& scene)const {
	if (scene.GetModelCount() <= 0)return;
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	const glm::mat4 activeInverseTransformation = glm::inverse(activeModel->GetWorldTransformation());
	 glm::vec3 WorldXAxixDirection = (activeModel->CalculateInModelFrameDirection(glm::vec3(1, 0, 0)));
	 glm::vec3 WorldYAxisDirection = (activeModel->CalculateInModelFrameDirection(glm::vec3(0, 1, 0)));
	 glm::vec3 WorldZAxisDirection = activeModel->CalculateInModelFrameDirection(glm::vec3(0, 0, 1));
	 glm::vec3 WorldOriginInModelFrame = activeModel->CalculateInModelFrame(glm::vec3(0, 0, 0));
	if (activeModel->getWorldThetaX() != GetWorldXRotation()) {
		const float dif = GetWorldXRotation() - activeModel->getWorldThetaX();
		const glm::mat4 rotateMatrix = Utils::RotateOrigin(DEGREETORADIAN(dif),X);
		activeModel->SetWorldTransformation(activeModel->GetWorldTransformation()*rotateMatrix);
		activeModel->setWorldThetaX(GetWorldXRotation());
	}
	else if (activeModel->getWorldThetaY() != GetWorldYRotation()) {
		const float dif = GetWorldYRotation() - activeModel->getWorldThetaY();
		const glm::mat4 rotateMatrix = Utils::RotateOrigin(DEGREETORADIAN(dif),Y);
		activeModel->SetWorldTransformation(activeModel->GetWorldTransformation()*rotateMatrix);
		activeModel->setWorldThetaY(GetWorldYRotation());
	}
	else if (activeModel->getWorldThetaZ() != GetWorldZRotation()) {
		const float dif = GetWorldZRotation() - activeModel->getWorldThetaZ();
		const glm::mat4 rotateMatrix = Utils::RotateOrigin(DEGREETORADIAN(dif),Z);
		activeModel->SetWorldTransformation(activeModel->GetWorldTransformation()*rotateMatrix);
		activeModel->setWorldThetaZ(GetWorldZRotation());
	}
}

void Renderer::UpdateModelRotation(const Scene& scene) const {
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	if (activeModel->getModelThetaX() != GetXAxisRotation()) {
		const float dif = GetXAxisRotation() - activeModel->getModelThetaX();
		activeModel->setModelThetaX(GetXAxisRotation());
		activeModel->SetModelTransformation(activeModel->GetModelTransformation()*Utils::RotateOrigin(DEGREETORADIAN(dif), X));
	}
	else if (activeModel->getModelThetyaY() != GetYAxisRotation()) {
		const float dif = GetYAxisRotation() - activeModel->getModelThetyaY();
		activeModel->setModelThetaY(GetYAxisRotation());
		activeModel->SetModelTransformation(activeModel->GetModelTransformation()*Utils::RotateOrigin(DEGREETORADIAN(dif), Y));
	}
	else if (activeModel->getModelThetaZ() != GetZAxisRotation()) {
		const float dif = GetZAxisRotation() - activeModel->getModelThetaZ();
		activeModel->setModelThetaZ(GetZAxisRotation());
		activeModel->SetModelTransformation(activeModel->GetModelTransformation()*Utils::RotateOrigin(DEGREETORADIAN(dif), Z));
	}
}

void Renderer::UpdateModelColor(const Scene& scene)const {
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	const glm::vec3& col = GetMeshColor();
	activeModel->SetColor(glm::vec4(col.x,col.y,col.z,1));
}

void Renderer::PrintModel(const Scene& scene,std::shared_ptr<const MeshModel> activeModel,glm::mat4 transform, const glm::vec3& col) {
	for (int j = 0; j < activeModel->GetFaceCount(); j++) {
		std::vector<glm::vec3> vertix = activeModel->GetVertices(j);
		Renderer::PrintTraingle(scene,vertix, transform, col);
//	for (int i = 0; i < 3; i++) {
//			Renderer::PrintLine(vertix.at(i), vertix.at((i + 1) % 3)- vertix.at(i), transform, glm::vec3(1,0,0));
			
//	}
		vertix.clear();
	}
}

void Renderer::PrintAllModels(const Scene& scene) {
	if (scene.GetModelCount() <= 0) return;
	Camera activeCamera = scene.GetActiveCamera();
	glm::mat4 IvT = activeCamera.GetInverseViewTranform();
	glm::mat4 pT = activeCamera.GetProjectionTransform();
	glm::mat4 VpT = Renderer::GetViewPortTramsform();
	for (int i = 0; i < scene.GetModelCount(); i++) {
		std::shared_ptr<const MeshModel> model = scene.GetModelIndex(i);
		glm::mat4 modelT = model->GetModelTransformation();
		glm::mat4 worldT = model->GetWorldTransformation();
		glm::mat4 finalM = glm::transpose(VpT)*glm::transpose(pT)*glm::transpose(IvT)*glm::transpose(worldT)*glm::transpose(modelT);
		glm::vec4 col = model->GetColor();
		Renderer::PrintModel(scene,model, finalM,glm::vec3(col.x,col.y,col.z));
	}
}

void Renderer::PrintAllCameras(const Scene& scene) {
	if (scene.GetCameraCount() <= 0) return;
	Camera activeCamera = scene.GetActiveCamera();
	glm::mat4 IvT = activeCamera.GetInverseViewTranform();
	glm::mat4 pT = activeCamera.GetProjectionTransform();
	glm::mat4 VpT = Renderer::GetViewPortTramsform();
	for (int i = 0; i < scene.GetCameraCount(); i++) {
		if (i == scene.GetActiveCameraIndex()) continue;
		Camera cam = scene.GetCameraIndex(i);
		std::shared_ptr<MeshModel> camModel = std::make_shared<MeshModel>(Utils::getCameraModel());
		glm::mat4 finalM= glm::transpose(VpT)*glm::transpose(pT)*glm::transpose(IvT)*glm::transpose(Utils::Scale(glm::vec3(2, 2, 2)))*glm::transpose(cam.GetViewTransform())
			;
		Renderer::PrintModel(scene,camModel, finalM,glm::vec3(0.25,0.25,0.25));
	}
}

void Renderer::PrintTraingle(const Scene& scene,const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,const glm::vec3& col) {
	int xMax = max(p1.x, p2.x, p3.x), yMax = max(p1.y,p2.y,p3.y);
	int xMin = min(p1.x, p2.x, p3.x), yMin = min(p1.y,p2.y,p3.y);
	if (xMax == xMin || yMax == yMin) {return; }
	if (xMax > viewportWidth-1) xMax = viewportWidth-1;
	if (yMax > viewportHeight-1) yMax=viewportHeight-1;
	if (xMin < 1) xMin = 1;
	if (yMin < 1) yMin = 1;
	for (int i = xMin; i <= xMax; i++) {
		for (int j = yMin; j <= yMax;j++) {
			if (Utils::DoesContain(glm::vec2(i, j), glm::vec2(p1.x, p1.y), glm::vec2(p2.x, p2.y), glm::vec2(p3.x, p3.y))) {
				float z = Utils::ZInterpolation(i,j,p1,p2,p3);
				if (z< getZ(i,j)) {
					Renderer::putZ(i, j, z);
					const glm::vec3 newColor = calculateColor(scene, glm::vec3(i, j, z), col);
					Renderer::putPixel(i, j, newColor);
				}
			}
		}
	}
}

void Renderer::PrintTraingle(const Scene& scene, const std::vector<glm::vec3>& vertix, glm::mat4 transform,const glm::vec3& col) {
	if (vertix.size() != 3) { std::cout << 123123 << std::endl; return; }
	const glm::vec3 p1 = transform*Utils::HomCoordinats(vertix.at(0));
	const glm::vec3 p2 = transform*Utils::HomCoordinats(vertix.at(1));
	const glm::vec3 p3 = transform*Utils::HomCoordinats(vertix.at(2));
	Renderer::PrintTraingle(scene,p1, p2,p3,col);
}

glm::vec3 Renderer::calculateColor(const Scene& scene, const glm::vec3& position, const glm::vec3& materialColor)const {
	glm::vec3 newColor = materialColor;
	//ambiet color
	newColor *=scene.getAmbient();
	//difuse

	//specular

	return newColor;
}