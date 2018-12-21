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
#define DEGREETORADIAN(theta)  (2 * theta / 360.0f*M_PI)
#define WORLDFRAMEAXIS 30.0f
#define MODELFRAMAXIS 15.0f
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
	if (scene.GetModelCount() <= 0) return;
	std::shared_ptr< MeshModel> activeModel = scene.GetAciveModel();
	static int _r = 0;
	Camera activeCamera = scene.GetActiveCamera();
	activeCamera.SetCameraLookAt(GetEye(),GetAt(),GetY());
	Renderer::UpdateWorldTransform(scene);
	glm::mat4 modelT = activeModel->GetModelTransformation();
	glm::mat4 worldT = activeModel->GetWorldTransformation();
	glm::mat4 IvT = activeCamera.GetInverseViewTranform();
	glm::mat4 pT = activeCamera.GetProjectionTransform();
	glm::mat4 VpT = Renderer::GetViewPortTramsform();
	std::vector<glm::vec3> vertix;
	glm::mat4 finalM = glm::transpose(VpT)*glm::transpose(pT)*glm::transpose(IvT)*glm::transpose(worldT)*glm::transpose(modelT);
	//print world frame
	Renderer::PrintWorldFrame(scene);
	//print model frame
	Renderer::PrintModelFrame(scene);
	//print model
	for (int i = 0; i < activeModel->GetFaceCount(); i++) {
		vertix = activeModel->GetVertices(i);
		float x[3], y[3];
		glm::vec4 transformedV[3];
		if (vertix.size() == 3) {
			for (int k = 0; k < 3; k++){
				transformedV[k] = finalM*Utils::HomCoordinats(vertix.at(k));
				//transformedV[k] = glm::transpose(worldT*Utils::Scale(glm::vec3(20000, 20000, 20000))*IvT*pT*Utils::Translate(glm::vec3(300, 300, 300)))*Utils::HomCoordinats(vertix.at(k));
				x[k] = transformedV[k].x;
				y[k] = transformedV[k].y;
			}
			drawTraingle(x[0], y[0], x[1], y[1], x[2], y[2], GetMeshColor());
		}
		vertix.clear();
	}
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
	//debuging
	if (getTemp()) {
		static int x = 0;
		const glm::mat4 worldT = glm::inverse(activeModel->GetWorldTransformation());
		glm::vec3 Origin = Utils::SwitchFromHom(glm::transpose(worldT) * Utils::HomCoordinats(glm::vec3(0, 0, 0)));
		//std::cout << Origin.x << " " << Origin.y << " " << Origin.z << std::endl;
		if (x == 0) {
			activeModel->SetWorldTransformation(Utils::Translate(Origin)*activeModel->GetWorldTransformation());
		}
		x++;
	}
	/*static int debug = 0;
	if (debug++ == 0) {
		const glm::mat4 temp = Utils::RotateAround(M_PI/2, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
		const glm::vec4 t = glm::transpose(temp)*glm::vec4(0, 0,  1, 1);
		std::cout << Utils::floatPresice(t.x )<< " " << t.y << " " << Utils::floatPresice(t.z) << " " << std::endl;
		std::cout << Utils::floatPresice(t.x*t.x + t.y*t.y + t.z*t.z) << std::endl;
	}*/
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
	Renderer::UpdateModelTranslate(scene);
	Renderer::UpdateWorldTranslate(scene);
	Renderer::UpdateModelRotation(scene);
	Renderer::UpdateModelScale(scene);
	Renderer::UpdateWorldRotation(scene);
}
void Renderer::UpdateViewTransform(const Scene& scene) const {
	//TODO
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
glm::mat4 Renderer::GetViewPortTramsform()const {
	glm::mat4 scaleM = Utils::Scale(glm::vec3(viewportWidth/2, viewportHeight/2,1));
	glm::mat4 translateM = Utils::Translate(glm::vec3(viewportWidth / 2, viewportHeight / 2, 0));
	return  scaleM*translateM;
}
void Renderer::PrintLineBresenham(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& color) {
	Renderer::PrintLineBresenham(point1.x,point1.y,point2.x,point2.y, color);
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
	Camera activeCamera = scene.GetActiveCamera();
	activeCamera.SetCameraLookAt(GetEye(), GetAt(), GetY());
	glm::mat4 IvT = activeCamera.GetInverseViewTranform();
	glm::mat4 pT = activeCamera.GetProjectionTransform();
	glm::mat4 VpT = Renderer::GetViewPortTramsform();
	Renderer::PrintFrame(WORLDFRAMEAXIS, glm::transpose(VpT)*glm::transpose(pT)*glm::transpose(IvT));
}
void Renderer::PrintModelFrame(const Scene& scene) {
	glm::vec3 scale = GetScale();
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	Camera activeCamera = scene.GetActiveCamera();
	activeCamera.SetCameraLookAt(GetEye(), GetAt(), GetY());
	glm::mat4 wT = activeModel->GetWorldTransformation();
	wT =/* Utils::Scale(glm::vec3(1 / scale.x, 1 / scale.y, 1 / scale.z))*/wT;
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
	glm::vec3 temp = GetScale();
	activeModel->SetModelTransformation(activeModel->GetModelTransformation()*(Utils::Translate(glm::vec3(difX, difY, difZ))));
	activeModel->setModelTranslate(newTranslate);
}
void Renderer::UpdateWorldTranslate(const Scene& scene)const {
	std::shared_ptr<MeshModel> activeModel = scene.GetAciveModel();
	const glm::vec3 translateVector = activeModel->getWorldTranslate();
	const glm::vec3 newTranslateVector = GetWorldTranslateVector();
	const glm::vec3 dif = newTranslateVector - translateVector;
	//glm::vec3 temp = GetScale();
	//temp.x = dif.x / temp.x; temp.y = dif.y / temp.y; temp.z = dif.z / temp.z;
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
		//std::cout << WorldXAxixDirection.x << " " << WorldXAxixDirection.y << " " << WorldXAxixDirection.z << " S" << std::endl;
		const glm::mat4 rotateMatrix = Utils::RotateOrigin(DEGREETORADIAN(dif),X);
		//const glm::mat4 rotateMatrix = Utils::Translate(WorldOriginInModelFrame*-1.0f)*Utils::RotateOrigin(dif,X)*Utils::Translate(WorldOriginInModelFrame);
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
	static int x = 0;
	if (x++ == 0)
		activeModel->SetWorldTransformation(Utils::RotateAround(getTemp(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))*activeModel->GetWorldTransformation());
}