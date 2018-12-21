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

bool showDemoWindow = false;
bool showAnotherWindow = true;

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

		ImGui::Text("Muhammad && Mohsen");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &showAnotherWindow);
		ImGui::Checkbox("Another Window22", &showAnotherWindow);
		ImGui::Checkbox("Another Window33", &showAnotherWindow);
		ImGui::Checkbox("Another Window44", &showAnotherWindow);
		ImGui::Checkbox("Another Window55", &showAnotherWindow);



		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}


	// 3. Show another simple window.
	if (showAnotherWindow)
	{
		ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
		{
			showAnotherWindow = false;
		}
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
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	//selecting color
	{
		static float color[3] = { 0,0,0 };
		ImGui::ColorEdit3("Color", color);
		meshColor.x = color[0];
		meshColor.y = color[1];
		meshColor.z = color[2];
	}

	//rotations in model frame
	{
		//X axis
		/*static float ThetaX = 0.0f;*/
		ImGui::SliderFloat("X rotate Model Frame",&rotateX,0.0f,360.0f);
		//Y axis
		static float ThetaY = 0.0f;
		ImGui::SliderFloat("Y rotate Model Frame", &rotateY, 0.0f, 360.0f);
		//Z axis
		static float ThetaZ = 0.0f;
		ImGui::SliderFloat("Z rotate Model Frame", &rotateZ, 0.0f, 360.0f);
	}
	//rotation in world frame
	{
		ImGui::SliderFloat("X rotate World Frame", &worldRotateX, 0.0f, 360.0f);
		ImGui::SliderFloat("Y rotate World Frame", &worldRotateY, 0.0f, 360.0f);
		ImGui::SliderFloat("Z rotate World Frame", &worldRotateZ,0.0f,360.0f);
	}
	//draw normal per vertix
	{
		ImGui::Checkbox("Normal Per Vertix",&normalPerVertix);
	}
	//draw normal per face
	{
		ImGui::Checkbox("Normal Per Face", &normalPerFace);
	}
	//draw bounding box
	{
		ImGui::Checkbox("Bounding Box", &boundingBox);
	}
	//sclae model
	{
		ImGui::Text("Model Frame Scale");
		ImGui::InputFloat3("", scale);
	}
	//camera zoom
	{
		ImGui::InputFloat("Zoom", &zoom);
	}
	//change look at
	{
		ImGui::Text("Look at");
		
		ImGui::InputFloat3("eye", eye);
		ImGui::InputFloat3("at", at);
		ImGui::InputFloat3("y axis", y);
	}
	//translate in world frame
	{
		ImGui::Text("Model Translate");
		ImGui::InputFloat3("translate", translate);
	}
	//
	{
		ImGui::Text("Temp");
		if (ImGui::Button("click")) {
			temp =!temp;
		}
	}
	//world frame translate
	{
		ImGui::Text("World Translate");
		ImGui::InputFloat3("world translate", WorldTranslate);
	}
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





