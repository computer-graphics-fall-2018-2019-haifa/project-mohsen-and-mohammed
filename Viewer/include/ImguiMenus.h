#pragma once
#include <imgui/imgui.h>
#include "Scene.h"

void DrawImguiMenus(ImGuiIO& io, Scene& scene);
const glm::vec4& GetClearColor();
const glm::vec3& GetMeshColor();
float GetXAxisRotation();
float GetYAxisRotation();
float GetZAxisRotation();
bool DrawFaceNormal();
bool DrawVertixNormal();
bool DrawBoundingBox();
