#pragma once

#include "GameEngine/Component/Meshes/StandardMaterial.h"
#include "Common/Events/Event.h"

#include <memory>
#include <queue>

namespace GameEngine
{
	class MaterialsEventQueue
	{
	public:
		template<class Type>
		using Pointer = std::shared_ptr<Type>;

	public:
		static Common::Event<MaterialsEventQueue, std::string, const StandardMaterial&> OnCreate;
		static Common::Event<MaterialsEventQueue, std::string, const StandardMaterial&> OnUpdate;
		static Common::Event<MaterialsEventQueue, std::string, const StandardMaterial&> OnDelete;

	public:
		static void Flush();

	public:
		static void Create(const Pointer<StandardMaterial>& material);
		static void Update(const Pointer<StandardMaterial>& material);
		static void Delete(const Pointer<StandardMaterial>& material);

	private:
		static std::queue<Pointer<StandardMaterial>> s_createQueue;
		static std::queue<Pointer<StandardMaterial>> s_updateQueue;
		static std::queue<Pointer<StandardMaterial>> s_deleteQueue;
	};
}