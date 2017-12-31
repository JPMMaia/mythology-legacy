#pragma once

#include "d3dx12.h"
#include "DirectX12Engine/Utilities/DirectXHelper.h"

#include <limits>
#include <minwinbase.h>
#include <wrl/client.h>

namespace DirectX12Engine
{
	template <class T>
	class GPUAllocator
	{
	public:
		typedef T value_type;

	public:
		GPUAllocator() = default;

		template <class U>
		constexpr GPUAllocator(const GPUAllocator<U>& other) noexcept :
			m_d3dDevice(other.D3DDevice()),
			m_isConstantBuffer(other.IsConstantBuffer())
		{
		}

		explicit GPUAllocator(ID3D12Device* d3dDevice, bool isConstantBuffer) :
			m_d3dDevice(d3dDevice),
			m_isConstantBuffer(isConstantBuffer)
		{
		}

	public:
		T* allocate(std::size_t n)
		{
			if (n > std::size_t(-1) / GetElementSize())
				throw std::bad_alloc();

			return CreateUploadBuffer(n);
		}

		void deallocate(T* p, std::size_t n) noexcept
		{
			if (m_uploadBuffer != nullptr)
			{
				m_uploadBuffer->Unmap(0, nullptr);
				m_uploadBuffer = nullptr;
			}
		}

		ID3D12Device* D3DDevice() const
		{
			return m_d3dDevice;
		}

		bool IsConstantBuffer() const
		{
			return m_isConstantBuffer;
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(std::size_t index) const
		{
			return m_uploadBuffer->GetGPUVirtualAddress() + index * GetElementSize();
		}

	private:
		T* CreateUploadBuffer(std::size_t size)
		{
			m_uploadBuffer.Reset();

			auto bufferSize = GetElementSize() * size;

			auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
			auto bufferDescription = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
			DX::ThrowIfFailed(
				m_d3dDevice->CreateCommittedResource(
					&uploadHeapProperties,
					D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
					&bufferDescription,
					D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(m_uploadBuffer.GetAddressOf())
				)
			);

			T* pointer;
			CD3DX12_RANGE readRange(0, 0);
			DX::ThrowIfFailed(m_uploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pointer)));
			ZeroMemory(pointer, bufferSize);

			return pointer;
		}

	public:
		std::size_t GetElementSize() const
		{
			return m_isConstantBuffer ? static_cast<std::size_t>(DX::CalculateConstantBufferByteSize(sizeof(T))) : sizeof(T);
		}

	private:
		ID3D12Device* m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadBuffer;
		bool m_isConstantBuffer;
	};

	template <class T, class U>
	bool operator==(const GPUAllocator<T>& a, const GPUAllocator<U>& b)
	{
		return false;
	}

	template <class T, class U>
	bool operator!=(const GPUAllocator<T>& a, const GPUAllocator<U>& b)
	{
		return !(a == b);
	}
}

namespace DirectX12Engine
{
	template <class T>
	class gpu_vector_iterator
	{
	public:
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using iterator_category = std::forward_iterator_tag;

	public:
		gpu_vector_iterator() = default;
		~gpu_vector_iterator() = default;
		gpu_vector_iterator(const gpu_vector_iterator& other) = default;
		gpu_vector_iterator(gpu_vector_iterator&& other) noexcept = default;
		gpu_vector_iterator(value_type* location) noexcept :
			m_location(location)
		{
		}

	public:

		bool operator==(const gpu_vector_iterator& other)
		{
			return m_location == other.m_location;
		}
		bool operator!=(const gpu_vector_iterator& other)
		{
			return !(*this == other);
		}

		reference operator*()
		{
			return *m_location;
		}
		const_reference operator*() const
		{
			return *m_location;
		}
		pointer operator->()
		{
			return m_location;
		}

		gpu_vector_iterator& operator++()
		{
			++m_location;
			return *this;
		}
		gpu_vector_iterator operator++(int)
		{
			gpu_vector_iterator other(*this);
			operator++();
			return other;
		}

		gpu_vector_iterator& operator--()
		{
			--m_location;
			return *this;
		}
		gpu_vector_iterator operator--(int)
		{
			gpu_vector_iterator other(*this);
			operator--();
			return other;
		}

		gpu_vector_iterator& operator+=(difference_type n)
		{
			m_location += n;
			return *this;
		}
		gpu_vector_iterator operator+(difference_type n)
		{
			auto temp = *this;
			return temp += n;
		}
		gpu_vector_iterator& operator-=(difference_type n)
		{
			return *this += -n;
		}
		gpu_vector_iterator operator-(difference_type n)
		{
			auto temp = *this;
			return temp -= n;
		}
		difference_type operator-(const gpu_vector_iterator& other)
		{
			return m_location - other.m_location;
		}

		reference operator[](difference_type n)
		{
			return *(this + n);
		}

		bool operator<(const gpu_vector_iterator& other)
		{
			return other - *this > 0;
		}
		bool operator>(const gpu_vector_iterator& other)
		{
			return other < *this;
		}
		bool operator>=(const gpu_vector_iterator& other)
		{
			return !(*this < other);
		}
		bool operator<=(const gpu_vector_iterator& other)
		{
			return !(*this > other);
		}

	private:
		value_type* m_location;
	};

	template<class T>
	gpu_vector_iterator<T> operator+(typename gpu_vector_iterator<T>::diference_type n, const gpu_vector_iterator<T>& iterator)
	{
		return iterator + n;
	}

	template <class value_type, class allocator_type>
	class gpu_vector
	{
	public:
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
		using iterator = gpu_vector_iterator<value_type>;
		using const_iterator = gpu_vector_iterator<const value_type>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public:
		gpu_vector() = default;

		explicit gpu_vector(std::size_t size, allocator_type&& allocator) :
			m_allocator(allocator)
		{
			resize(size);
		}
		explicit gpu_vector(allocator_type&& allocator) :
			m_allocator(allocator)
		{
		}

		~gpu_vector()
		{
			clear();
		}

	public:
		template<class InputIterator>
		void assign(InputIterator first, InputIterator last)
		{
			auto count = std::distance(first, last);

			if(count != m_size)
			{
				clear();
				resize(count);
			}

			auto dataIterator = begin();
			for(auto copyIterator = first; copyIterator != last; ++copyIterator)
			{
				*dataIterator = *copyIterator;
				++dataIterator;
			}

			m_size = count;
		}

		void push_back(const value_type& value)
		{
			// Reserve more space if needed:
			if (m_capacity == m_size)
				reserve(calculate_growth(m_size + 1));

			// Set value:
			m_data[m_size] = value;

			// Increase size:
			++m_size;
		}
		void push_back(value_type&& value)
		{
			emplace_back(std::forward<value_type>(value));
		}

		template<typename... Args>
		void emplace_back(Args&& ...args)
		{
			// Reserve more space if needed:
			if (m_capacity == m_size)
				reserve(calculate_growth(m_size + 1));

			// Construct object:
			new (static_cast<value_type*>(m_data + m_size)) value_type(std::forward<Args>(args)...);

			// Increase size:
			++m_size;
		}

		void clear()
		{
			if (m_data)
			{
				m_allocator.deallocate(m_data, m_capacity);
				m_data = nullptr;
				m_capacity = 0;
				m_size = 0;
			}
		}
		void reserve(std::size_t capacity)
		{
			if (capacity > m_capacity)
			{
				std::vector<value_type> copy(begin(), end());

				// Clear memory:
				clear();

				// Allocate new memory:
				m_data = m_allocator.allocate(capacity);
				m_capacity = capacity;

				// Copy data back to new allocated memory:
				m_size = copy.size();

				iterator dataIterator = begin();
				for (auto copyIterator = copy.cbegin(); copyIterator != copy.cend() && dataIterator != end(); ++copyIterator)
				{
					*dataIterator = *copyIterator;
					++dataIterator;
				}
			}
		}
		void resize(std::size_t count)
		{
			// If the current size is greater than count:
			if(m_size > count)
			{
				// Reduce the container to its first count elements:
				m_size = count;
			}

			// If the current size is less than count:
			else if(m_size < count)
			{
				reserve(calculate_growth(count));

				// Append aditional default-inserted elements:
				while(m_size != count)
					emplace_back();
			}
		}
		void shrink_to_fit()
		{
			// Do nothing if capacity is fully used:
			if (m_size == m_capacity)
				return;

			// Copy values to temporary memory:
			std::vector<value_type> copy(begin(), end());

			// Clear all memory space and reserve enough space for elements:
			auto size = m_size;
			clear();
			reserve(size);

			// Copy values back from temporary memory to new allocated memory:
			assign(copy.begin(), copy.end());
		}

	public:
		std::size_t capacity() const
		{
			return m_capacity;
		}
		std::size_t size() const
		{
			return m_size;
		}
		std::size_t max_size() const
		{
			return (std::numeric_limits<std::size_t>::max)();
		}
		bool empty() const
		{
			return m_size == 0;
		}

		const allocator_type& get_allocator() const
		{
			return m_allocator;
		}

	public:
		iterator begin() noexcept
		{
			return iterator(m_data);
		}
		const_iterator begin() const noexcept
		{
			return const_iterator(m_data);
		}
		const_iterator cbegin() const noexcept
		{
			return begin();
		}
		iterator end() noexcept
		{
			return iterator(m_data + m_size);
		}
		const_iterator end() const noexcept
		{
			return const_iterator(m_data + m_size);
		}
		const_iterator cend() const noexcept
		{
			return end();
		}

	public:
		value_type& operator[](std::size_t index)
		{
			if (index >= m_size)
				throw std::out_of_range("Index out of range!");

			return m_data[index];
		}

	private:
		std::size_t calculate_growth(std::size_t newSize) const
		{
			const auto oldCapacity = m_capacity;

			if (oldCapacity > max_size() - oldCapacity / 2)
			{
				// Geometric growth would overflow:
				return newSize;
			}

			const auto geometric = oldCapacity + oldCapacity / 2;

			if (geometric < newSize)
			{
				// Geometric growth would be insufficient:
				return (newSize);
			}

			// Geometric growth is sufficient:
			return geometric;
		}

	private:
		value_type* m_data = nullptr;
		std::size_t m_capacity = 0;
		std::size_t m_size = 0;
		allocator_type m_allocator;
	};

	template <class DataType>
	using GPUUploadBuffer = gpu_vector<DataType, GPUAllocator<DataType>>;
}
