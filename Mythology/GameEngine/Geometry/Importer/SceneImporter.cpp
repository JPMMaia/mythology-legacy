#include "SceneImporter.h"
#include "Libraries/assimp/Importer.hpp"
#include "Libraries/assimp/scene.h"
#include "Libraries/assimp/postprocess.h"

using namespace Assimp;
using namespace GameEngine;

void SceneImporter::Test()
{
	Assimp::Importer importer;

	auto flags =
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_SortByPType;
	auto scene = importer.ReadFile("Resources/Box.gltf", flags);

	if (!scene)
		throw std::invalid_argument("Can't import given scene.");
}
