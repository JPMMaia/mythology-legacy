#pragma once

#include "PhysicsEngine/IScene.h"
#include "PhysicsEngine/Actors/IActorFactory.h"
#include "PhysicsEngine/Geometries/IGeometryFactory.h"
#include "PhysicsEngine/Materials/IMaterialFactory.h"

#include <memory>

namespace PhysicsEngine
{	
	template<class T = class PhysicsTraitsImplementation>
	class IManager
	{
	public:
		virtual ~IManager() = default;

	public:
		virtual std::unique_ptr<IScene<T>> CreateScene() = 0;
		virtual std::unique_ptr<IGeometryFactory<T>> CreateGeometryFactory() = 0;
		virtual std::unique_ptr<IMaterialFactory<T>> CreateMaterialFactory() = 0;
		virtual std::unique_ptr<IActorFactory<T>> CreateActorFactory() = 0;
	};
}
