#pragma once

#include "Libraries/Eigen/Geometry"

namespace GameEngine
{
	template<typename ValueTypeT>
	struct Keyframe
	{
	public:
		using ValueType = ValueTypeT;

	public:
		Keyframe() :
			TimePosition(0.0f)
		{
		}
		Keyframe(float timePosition, const ValueType& value) :
			TimePosition(timePosition),
			Value(value)
		{
		}

	public:
		float TimePosition;
		ValueType Value;
	};
}

namespace std
{
	template<typename T>
	struct less<GameEngine::Keyframe<T>>
	{
	public:
		using value_type = GameEngine::Keyframe<T>;

		constexpr bool operator()(const value_type& lhs, const value_type& rhs) const
		{
			return lhs.TimePosition < rhs.TimePosition;
		}
	};
}