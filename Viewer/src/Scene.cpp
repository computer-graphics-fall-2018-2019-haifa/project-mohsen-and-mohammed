#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(0),
	activeModelIndex(0)
{

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model)
{
	models.push_back(model);
}

const int Scene::GetModelCount() const
{
	return models.size();
}

void Scene::AddCamera(const Camera& camera)
{
	cameras.push_back(camera);
}

const int Scene::GetCameraCount() const
{
	return cameras.size();
}

void Scene::SetActiveCameraIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < cameras.size())
	{
		activeCameraIndex = index;
	}
}

const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}

void Scene::SetActiveModelIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < models.size())
	{
		activeModelIndex = index;
	}
}

const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}



const std::vector<Face>& Scene::GetModelFaces(int index) const
{
	return models[index].get()->GetModelFaces();
}


/*const std::vector<glm::vec3>& Scene::GetVertices(int index) const
{
	auto a = models[index];
	return a.get();
	return models[index].get()->GetVertices();
}*/
/*
glm::mat4(1,0,0,0
		0,1,0,0
		0,0,0,0
		0,0,0,0)*/


const MeshModel& Scene::GetModel(int index) const
{
	auto a = models[index];
	return *a.get();

}
