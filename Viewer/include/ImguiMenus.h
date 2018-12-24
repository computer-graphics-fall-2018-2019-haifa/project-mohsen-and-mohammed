#pragma once
#include <imgui/imgui.h>
#include "Scene.h"

void DrawImguiMenus(ImGuiIO& io, Scene& scene);
const glm::vec4& GetClearColor();
const glm::vec3& GetMeshColor();
glm::vec3 GetTranslateVector();
glm::vec3 GetEye();
glm::vec3 GetAt();
glm::vec3 GetY();
float GetXAxisRotation();
float GetYAxisRotation();
float GetZAxisRotation();
float GetWorldXRotation();
float GetWorldYRotation();
float GetWorldZRotation();
glm::vec3 GetScale();
glm::vec3 GetWorldTranslate();
bool DrawFaceNormal();
bool DrawVertixNormal();
bool DrawBoundingBox();
bool getTemp();
glm::vec3 GetWorldTranslateVector();
float GetCamXRotate();
float GetCamYRotate();
float GetCamZRotate();
