#pragma once

#include "StandardAllocator.h"

#include <memory>

namespace GameEngine
{
	template<class Type>
	class UseStorage
	{
	public:
		using Allocator = StandardAllocator<Type>;

	public:
		static void Clear()
		{
			s_storage.clear();
			s_storage.shrink_to_fit();
		}

		static void Deleter(void* rawPointer) noexcept
		{
			s_storage.deallocate(reinterpret_cast<typename Allocator::value_type*>(rawPointer), sizeof(Type));
		}

		template<class... Args>
		static std::shared_ptr<Type> CreateSharedPointer(Args... args)
		{
			// Allocate memory for object:
			auto rawPointer = s_storage.allocate(sizeof(Type));

			// Construct object:
			::new (static_cast<void*>(rawPointer)) Type(std::forward<Args>(args)...);

			// Wrap in a shared pointer and return:
			return std::shared_ptr<Type>(rawPointer, Deleter);
		}
		static StandardAllocatorIterator<Type> begin()
		{
			return s_storage.begin();
		}
		static StandardAllocatorIterator<Type> end()
		{
			return s_storage.end();
		}
		static const StandardAllocator<Type>& GetStorage()
		{
			return s_storage;
		}

	private:
		static StandardAllocator<Type> s_storage;
	};

	template<class Type>
	typename UseStorage<Type>::Allocator UseStorage<Type>::s_storage;
}
