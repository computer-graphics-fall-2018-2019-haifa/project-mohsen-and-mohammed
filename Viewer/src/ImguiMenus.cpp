#pragma once
#define _USE_MATH_DEFINES

#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Utils.h"
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <nfd.h>
#include <random>
#include <iostream>
#define DEGREETORADIAN(theta)  (2 * theta / 360.0f*M_PI)
bool showDemoWindow = false;
bool showAnotherWindow = true;
bool showModelWindow = true;
bool showSeneWindow = true;
bool showCameraWindow = true;
bool perspective = false;
bool orthographic = !perspective;
glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
glm::vec3 meshColor = glm::vec3(0.0f, 0.0f, 0.0f);
static bool temp = false;
static float worldRotateX = 0.0f;
static float worldRotateY = 0.0f;
static float worldRotateZ = 0.0f;
static float rotateX = 0.0f;
static float rotateY = 0.0f;
static float rotateZ = 0.0f;
static bool normalPerFace = false;
static bool normalPerVertix = false;
static bool boundingBox = false;
float zoom = 1;
static float eye[3] = {0,0,10};
static float at[3] = {0,0,0};
static float y[3] = {0,1,0};
static float translate[3] = { 0,0,0 };
static float scale[3] = { 1,1,1 };
static float WorldTranslate[3] = { 0,0,0 };

static float cameraRotateX = 0.0f;
static float cameraRotateY = 0.0f;
static float cameraRotateZ = 0.0f;

static float fovy = 50.0f;
static float aspect_p = 2.5f;
static float near_p = 5.0f;
static float far_p = -5.0f;

static float height = 100.0f;
static float aspect_o = 2.0f;
static float near_o = 5.0f;
static float far_o = -5.0f;

static bool printAllModels = false;
static bool printAllCameras = false;

static float tiltX = 0.0;
static float tiltY = 0.0;
static float tiltZ = 0.0;
/*
static void UpdateTiltX(Scene& scene) {
	Camera cam = scene.GetActiveCamera();
	const float dif = tiltX - cam.getTiltX();
	if (dif == 0) return;
	if (tiltX > -90 && tiltX < 90) {
		//the distance of the camera form X/Y axis is Z cordinate of eye vector
		//this equation holds:tanf(dif)=d/eye.z where d holds d+at=newat
		const float d = tanf(DEGREETORADIAN(dif))*cam.getEye().z;
		const glm::vec3 newAt = cam.getAt() + glm::vec3(d, 0, 0);
		scene.ActiveCameraLookAt(cam.getEye(), newAt, cam.getY());
		scene.UpdateActiveCameraTilt(tiltX, X);
		at[0] = newAt.x; at[1] = newAt.y; at[2] = newAt.z;
	}
	else if (tiltX == 90) {
		//if we tilt in 90 degrees the z axis of camera frame must be parallel to X axis
		const glm::vec3 newAt = cam.getEye() + glm::vec3(1, 0, 0);
		scene.ActiveCameraLookAt(cam.getEye(), newAt, cam.getY());
		scene.UpdateActiveCameraTilt(tiltX, X);
		at[0] = newAt.x; at[1] = newAt.y; at[2] = newAt.z;
	}
	else if (tiltX == -90) {
		const glm::vec3 newAt = cam.getEye() + glm::vec3(-1, 0, 0);
		scene.ActiveCameraLookAt(cam.getEye(), newAt, cam.getY());
		scene.UpdateActiveCameraTilt(tiltX, X);
		at[0] = newAt.x; at[1] = newAt.y; at[2] = newAt.z;
	}
	else if (tiltX>90) {
		const float d = tanf(DEGREETORADIAN(dif-90))*cam.getEye().z;
		const glm::vec3 newAt = cam.getAt() + glm::vec3(d, 2*GetEye().z, 0);
		scene.ActiveCameraLookAt(cam.getEye(), newAt, cam.getY());
		scene.UpdateActiveCameraTilt(tiltX, X);
		at[0] = newAt.x; at[1] = newAt.y; at[2] = newAt.z;
	}
	else if (tiltX < -90) {
		const float d = tanf(DEGREETORADIAN(90 + dif))*cam.getEye().z;
		const glm::vec3 newAt = cam.getAt() + glm::vec3(d, GetEye().z, 0);
		scene.ActiveCameraLookAt(cam.getEye(), newAt, cam.getY());
		scene.UpdateActiveCameraTilt(tiltX, X);
		at[0] = newAt.x; at[1] = newAt.y; at[2] = newAt.z;
	}
}
	
static void UpdateTiltY(Scene& scene) {
	Camera cam = scene.GetActiveCamera();
	const float dif = tiltY - cam.getTiltY();
	if (dif == 0) return;
	//the distance of the camera form X/Y axis is Z cordinate of eye vector
	//this equation holds:tanf(dif)=d/eye.z where d holds d+at=newat
	const float d = tanf(DEGREETORADIAN(dif))*cam.getEye().z;
	const glm::vec3 newAt = cam.getAt() + glm::vec3(0, d, 0);
	scene.ActiveCameraLookAt(cam.getEye(), newAt, cam.getY());
	scene.UpdateActiveCameraTilt(tiltY, Y);
	at[0] = newAt.x; at[1] = newAt.y; at[2] = newAt.z;
}
static void UpdateTiltZ(Scene& scene) {
	Camera cam = scene.GetActiveCamera();
	const float dif = tiltZ - cam.getTiltZ();
	if (dif == 0) return;
	//the distance of the camera form Z axis is X cordinate of eye vector
	//this equation holds:tanf(dif)=d/eye.z where d holds d+at=newat
	const float d = tanf(DEGREETORADIAN(dif))*cam.getEye().x;
	const glm::vec3 newAt = cam.getAt() + glm::vec3(0, 0, d);
	scene.ActiveCameraLookAt(cam.getEye(), newAt, cam.getY());
	scene.UpdateActiveCameraTilt(tiltZ, Z);
	at[0] = newAt.x; at[1] = newAt.y; at[2] = newAt.z;
}*/
static void UpdateLookAt(Scene& scene) {
	scene.ActiveCameraLookAt(GetEye(), GetAt(), GetY());
}
static void UpdateXRotate(Scene& scene) {
	const float  dif = GetCamXRotate() - scene.GetActiveCameraXRotate();
	if (dif == 0) return;
	const glm::vec3 newEye = Utils::SwitchFromHom(glm::inverse(Utils::RotateOrigin(DEGREETORADIAN(dif), X))*Utils::HomCoordinats(GetEye()));
	//const glm::vec3 newUp = Utils::SwitchFromHom(Utils::RotateOrigin(DEGREETORADIAN(dif), X)*Utils::HomCoordinats(GetY()));
	scene.ActiveCameraLookAt(newEye, GetAt(), GetY());
	scene.UpdateActiveCameraXRotate(GetCamXRotate());
	eye[0] = newEye.x; //y[0] = newUp.x;
	eye[1] = newEye.y;// y[1] = newUp.y;
	eye[2] = newEye.z; //y[2] = newUp.z;
}
static void UpdateYRotate(Scene& scene) {
	const float dif = GetCamYRotate() - scene.GetActiveCameraYRotate();
	if (dif == 0) return;
	const glm::vec3 newEye = Utils::SwitchFromHom(Utils::RotateOrigin(DEGREETORADIAN(dif), Y)*Utils::HomCoordinats(GetEye()));
	const glm::vec3 newUp = Utils::SwitchFromHom(Utils::RotateOrigin(DEGREETORADIAN(dif), Y)*Utils::HomCoordinats(GetY()));
	scene.ActiveCameraLookAt(newEye, GetAt(), GetY());
	scene.UpdateActiveCameraYRotate(GetCamYRotate());
	eye[0] = newEye.x; y[0] = newUp.x;
	eye[1] = newEye.y; y[1] = newUp.y;
	eye[2] = newEye.z; y[2] = newUp.z;
}
static void UpdateZRotate(Scene& scene) {
	const float dif = GetCamZRotate() - scene.GetActiveCameraZRotate();
	if (dif == 0) return;
	const glm::vec3 newEye = Utils::SwitchFromHom(Utils::RotateOrigin(DEGREETORADIAN(dif), Z)*Utils::HomCoordinats(GetEye()));
	scene.UpdateActiveCameraZRotate(GetCamZRotate());
	eye[0] = newEye.x;
	eye[1] = newEye.y;
	eye[2] = newEye.z;
}
static void UpdatePerspective(Scene& scene) {
	scene.ActiveCameraSerPers(fovy, aspect_p, near_p, far_p);
}
static void UpdateOrthographic(Scene& scene) {
	scene.ActiveCamerSetOrth(height, aspect_o, near_o, far_o);
}
static void UpdateActiveCamera(Scene& scene) {
	UpdateLookAt(scene);
	UpdateXRotate(scene);
	UpdateYRotate(scene);
	UpdateZRotate(scene);
	//UpdateTiltX(scene);
	//UpdateTiltY(scene);
	//UpdateTiltZ(scene);
	if (perspective) {
		UpdatePerspective(scene);
	}
	else if (orthographic) {
		UpdateOrthographic(scene);
	}
}
const glm::vec4& GetClearColor()
{
	return clearColor;
}
const glm::vec3& GetMeshColor() {
	return meshColor;
}
glm::vec3 GetTranslateVector() {
	return glm::vec3(translate[0], translate[1], translate[2]);
}
glm::vec3 GetEye() {
	return glm::vec3(eye[0], eye[1], eye[2]);
}
glm::vec3 GetAt() {
	return glm::vec3(at[0],at[1],at[2]);
}
glm::vec3 GetY() {
	return glm::vec3(y[0],y[1],y[2]);
}
float GetXAxisRotation() {
	return rotateX;
}
float GetYAxisRotation() {
	return rotateY;
}
float GetZAxisRotation() {
	return rotateZ;
}
glm::vec3 GetScale() {
	return glm::vec3(scale[0],scale[1],scale[2]);
}
glm::vec3 GetWorldTranslate() {
	return glm::vec3(translate[0], translate[1], translate[2]);
}
bool DrawFaceNormal() {
	return normalPerFace;
}
bool DrawVertixNormal() {
	return normalPerVertix;
}
bool DrawBoundingBox() {
	return boundingBox;
}
void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	Camera cam = scene.GetActiveCamera();
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		//ImGui::Text("Muhammad && Mohsen");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Scene Window", &showSeneWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Model Window", &showModelWindow);
		ImGui::Checkbox("Camera Window", &showCameraWindow);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 4. Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Model...", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene.AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath)));
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				if (ImGui::MenuItem("Add camera...","CTRL+C")) {
					scene.AddCamera(Camera(glm::vec4(0,0,0,1),glm::vec4(0,0,1,1),glm::vec4(0,1,0,1)));
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
	//model window
	if(showModelWindow){
		ImGui::Begin("Model Window");
		//selecting color
		static float color[3] = { 0,0,0 };
		ImGui::ColorEdit3("Color", color);
		meshColor.x = color[0];
		meshColor.y = color[1];
		meshColor.z = color[2];
		//rotations in model frame
			//X axis
		ImGui::SliderFloat("X rotate Model Frame", &rotateX, 0.0f, 360.0f);
		//Y axis
		static float ThetaY = 0.0f;
		ImGui::SliderFloat("Y rotate Model Frame", &rotateY, 0.0f, 360.0f);
		//Z axis
		static float ThetaZ = 0.0f;
		ImGui::SliderFloat("Z rotate Model Frame", &rotateZ, 0.0f, 360.0f);
	//rotation in world frame
		ImGui::SliderFloat("X rotate World Frame", &worldRotateX, 0.0f, 360.0f);
		ImGui::SliderFloat("Y rotate World Frame", &worldRotateY, 0.0f, 360.0f);
		ImGui::SliderFloat("Z rotate World Frame", &worldRotateZ, 0.0f, 360.0f);
	//draw normal per vertix
		ImGui::Checkbox("Normal Per Vertix", &normalPerVertix);
	//draw normal per face
		ImGui::Checkbox("Normal Per Face", &normalPerFace);
	//draw bounding box
		ImGui::Checkbox("Bounding Box", &boundingBox);
	//sclae model
		ImGui::Text("Model Frame Scale");
		ImGui::InputFloat3("", scale);
	
	//translate in world frame
		ImGui::Text("Model Translate");
		ImGui::InputFloat3("translate", translate);
	//world frame translate
		ImGui::Text("World Translate");
		ImGui::InputFloat3("world translate", WorldTranslate);
		ImGui::End();
	}
	if (showCameraWindow) {
		ImGui::Begin("Camera Window");
		if (ImGui::Button("focus active model")) {
			if (scene.GetModelCount() > 0) {
				std::shared_ptr< MeshModel> activeModel = scene.GetAciveModel();
				glm::mat4 modelT = activeModel->GetModelTransformation();
				glm::mat4 worldT = activeModel->GetWorldTransformation();
				glm::vec4 newAt = glm::transpose(worldT)*glm::transpose(modelT)*glm::vec4(0,0,0,1);
				at[0] = newAt.x; at[1] = newAt.y; at[2] = newAt.z;
			}
		}
		//camera zoom
		ImGui::InputFloat("Zoom", &zoom);
		//change look at
		ImGui::Text("Look at");
		ImGui::InputFloat3("eye", eye);
		ImGui::InputFloat3("at", at);
		ImGui::InputFloat3("y axis", y);
		//camera tilt
	/*	ImGui::Text("Camera Tilt");
		ImGui::InputFloat("Camera X Tilt", &tiltX);
		ImGui::SliderFloat("Camera Y Tilt", &tiltY, -360.0f, 360.0f);
		ImGui::SliderFloat("Camera Z Tilt", &tiltZ, -360.0f, 360.0f);*/
		//camera rotations
		ImGui::Text("Camera Rotations");
		ImGui::SliderFloat("Camera X Rotate", &cameraRotateX, 0.0f, 360.0f);
		ImGui::SliderFloat("Camera Y Rotate", &cameraRotateY, 0.0f, 360.0f);
		ImGui::SliderFloat("Camera Z Rotate", &cameraRotateZ, 0.0f, 360.0f);
		ImGui::Checkbox("Perspective", &perspective);
		ImGui::SameLine();
		orthographic = !perspective;
		ImGui::Checkbox("Orthographic", &orthographic);
		perspective = !orthographic;
		bool flag = true;
		if (orthographic) {
			ImGui::InputFloat("height",&height);
			ImGui::InputFloat("aspect ratio", &aspect_o);
			ImGui::InputFloat("near", &near_o);
			ImGui::InputFloat("far",&far_o);
		}
		else if(perspective){
			ImGui::InputFloat("fovy", &fovy);
			ImGui::InputFloat("aspect ratio", &aspect_p);
			ImGui::InputFloat("near",&near_p);
			ImGui::InputFloat("far", &far_p);
		}
		ImGui::End();
	}
	if (showSeneWindow) {
		ImGui::Begin("Scene Window");
		if (ImGui::Button("Switch Camera")) {
			if (scene.GetCameraCount() > 0) {
				scene.SetActiveCameraIndex((scene.GetActiveCameraIndex() + 1) % scene.GetCameraCount());
				ResetImGuiMenusCamera(scene.GetActiveCamera());
			}
		}
		if (ImGui::Button("Switch active Model")) {
			if (scene.GetModelCount() > 0){
				scene.SetActiveModelIndex((scene.GetActiveModelIndex()+1)%scene.GetModelCount());
				ResetImGuiMenusModel(scene.GetAciveModel());
			}
		}
		ImGui::Checkbox("Print All Models", &printAllModels);
		ImGui::Checkbox("Print All Cameras",&printAllCameras);
		ImGui::End();
	}
	//UpdateActiveCamera(scene);
}
bool getTemp() {
	return temp;
}
glm::vec3 GetWorldTranslateVector() {
	return glm::vec3(WorldTranslate[0], WorldTranslate[1], WorldTranslate[2]);
}
float GetWorldXRotation() {
	return worldRotateX;
}
float GetWorldYRotation() {
	return worldRotateY;
}
float GetWorldZRotation() {
	return worldRotateZ;
}
float GetCamXRotate() {
	return cameraRotateX;
}
float GetCamYRotate() {
	return cameraRotateY;
}
float GetCamZRotate() {
	return cameraRotateZ;
}
void ResetImGuiMenusModel() {
	worldRotateX = 0.0f;
	worldRotateY = 0.0f;
	worldRotateZ = 0.0f;
	rotateX = 0.0f;
	rotateY = 0.0f;
	rotateZ = 0.0f;
	normalPerFace = false;
	normalPerVertix = false;
	boundingBox = false;
	translate[0] = 0; translate[1] = 0; translate[2] = 0; 
	WorldTranslate[0] = 0; WorldTranslate[1] = 0; WorldTranslate[2] = 0; 
	scale[0] = 1; scale[1] = 1; scale[1] = 1; 
}
void ResetImGUiMenusCamera() {
	eye[0] = 0; eye[1] = 0; eye[2] = 10;
	at[0] = 0; at[1] = 0; at[2] = 0;
	y[0] = 0; y[1] = 1; y[2] = 0;
	cameraRotateX = 0.0f;
	cameraRotateY = 0.0f;
	cameraRotateZ = 0.0f;
	fovy = 50.0f;
	aspect_p = 2.5f;
	near_p = 5.0f;
	far_p = -5.0f;
	height = 100.0f;
	aspect_o = 2.0f;
	near_o = 5.0f;
	far_o = -5.0f;
	//tiltX = 0.0f;
	//tiltY = 0.0f;
	// tiltZ = 0.0f;
}
void ResetImGuiMenusModel(std::shared_ptr<const MeshModel> model) {
	worldRotateX = model->getWorldThetaX();
	worldRotateY = model->getWorldThetaY();
	worldRotateZ = model->getWorldThetaZ();
	rotateX = model->getModelThetaX();
	rotateY = model->getModelThetyaY();
	rotateZ = model->getModelThetaZ();
	normalPerFace = false;
	normalPerVertix = false;
	boundingBox = false;
	glm::vec3 trans = model->getModelTranslate();
	translate[0] = trans.x; translate[1] = trans.y; translate[2] = trans.z;
	glm::vec3 wtrans = model->getWorldTranslate();
	WorldTranslate[0] = wtrans.x; WorldTranslate[1] = wtrans.y; WorldTranslate[2] = wtrans.z;
	glm::vec3 wscl = model->getModelScale();
	scale[0] = wscl.x; scale[1] = wscl.y; scale[1] = wscl.z;
	glm::vec4 mCol = model->GetColor();
	meshColor.x = mCol.x;
	meshColor.y = mCol.y;
	meshColor.z = mCol.z;
}
void ResetImGuiMenusCamera(const Camera& camera) {
	glm::vec3 eye_ = camera.getEye();
	eye[0] = eye_.x; eye[1] = eye_.y; eye[2] = eye_.z;
	glm::vec3 at_ = camera.getAt();
	at[0] = at_.x; at[1] = at_.y; at[2] = at_.z;
	glm::vec3 y_ = camera.getY();
	y[0] = y_.x; y[1] = y_.y; y[2] = y_.z;
	cameraRotateX = camera.getXRotate();
	cameraRotateY = camera.getYRotate();
	cameraRotateZ = camera.getZRotate();
	fovy = camera.getFovy();
	aspect_p = camera.getAspectP();
	near_p = camera.getNearP();
	far_p = camera.getFarP();
	height = camera.getHeight();
	aspect_o = camera.getAspectO();
	near_o = camera.getNearO();
	far_o = camera.getFarO();
	//tiltX = camera.getTiltX();
	//tiltY = camera.getTiltY();
	//tiltZ = camera.getTiltZ();
}
bool PrintAllModels_() {
	return printAllModels;
}
bool PrintAllCameras_() {
	return printAllCameras;
}