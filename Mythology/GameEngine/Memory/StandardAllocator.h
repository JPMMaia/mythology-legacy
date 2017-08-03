#pragma once
#include <memory>
#include "GameEngine/GameManager.h"

namespace GameEngine
{
	template <class T>
	class StandardAllocator
	{
	public:
		typedef T value_type;

	public:
		static void SetGameManager(const std::shared_ptr<GameManager>& gameManager)
		{
			s_gameManager = gameManager;
		}

	public:
		StandardAllocator() = default;

		template <class U>
		constexpr StandardAllocator(const StandardAllocator<U>& other) noexcept
		{
		}

	public:
		T* allocate(std::size_t n)
		{
			if (n > std::size_t(-1) / sizeof(T))
				throw std::bad_alloc();


		}

		void deallocate(T* p, std::size_t n) noexcept
		{
			if (m_uploadBuffer != nullptr)
			{
				m_uploadBuffer->Unmap(0, nullptr);
				m_uploadBuffer = nullptr;
			}
		}

	private:
		static std::shared_ptr<GameManager> s_gameManager;
	};

	template <class T, class U>
	bool operator==(const StandardAllocator<T>& a, const StandardAllocator<U>& b)
	{
		return false;
	}

	template <class T, class U>
	bool operator!=(const StandardAllocator<T>& a, const StandardAllocator<U>& b)
	{
		return !(a == b);
	}
}
