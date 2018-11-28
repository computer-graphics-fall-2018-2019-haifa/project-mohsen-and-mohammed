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
bool showAnotherWindow = false;

glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
glm::vec3 meshColor = glm::vec3(0.0f, 0.0f, 0.0f);
float rotateX = 0.0f;
float rotateY = 0.0f;
float rotateZ = 0.0f;
bool normalPerFace = false;
bool normalPerVertix = false;
bool boundingBox = false;
float scale = 1;
float zoom = 1;
const glm::vec4& GetClearColor()
{
	return clearColor;
}

const glm::vec3& GetMeshColor() {
	return meshColor;
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
		ImGui::SliderFloat("X rotate",&rotateX,0.0f,360.0f);
		//Y axis
		static float ThetaY = 0.0f;
		ImGui::SliderFloat("Y rotate", &rotateY, 0.0f, 360.0f);
		//Z axis
		static float ThetaZ = 0.0f;
		ImGui::SliderFloat("Z rotate", &rotateZ, 0.0f, 360.0f);
		/*rotateX = ThetaX;
		rotateY = ThetaY;
		rotateZ = ThetaZ;*/
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
	//sclae mode
	{
		ImGui::InputFloat("Scale", &scale);
	}
	//camera zoom
	{
		ImGui::InputFloat("Zoom", &zoom);
	}
}






