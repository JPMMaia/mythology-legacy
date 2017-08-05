#pragma once

#include <cstddef>

namespace Common
{
	/// See http://www.codeproject.com/Articles/1083210/An-efficient-Cplusplus-fixed-block-memory-allocato
	class Allocator
	{
	public:
		/// Constructor
		/// @param[in]  size - size of the fixed blocks
		/// @param[in]  objects - maximum number of object. If 0, new blocks are
		///		created off the heap as necessary.
		/// @param[in]	memory - pointer to a block of static memory for allocator or NULL 
		///		to obtain memory from global heap. If not NULL, the objects argument 
		///		defines the size of the memory block (size x objects = memory size in bytes).
		///	@param[in]	name - optional allocator name string.
		explicit Allocator(std::size_t size, std::size_t objects = 0, char* memory = nullptr, const char* name = nullptr);

		/// Destructor
		~Allocator();

		/// Get a pointer to a memory block. 
		/// @param[in]  size - size of the block to allocate
		/// @return     Returns pointer to the block. Otherwise NULL if unsuccessful.
		void* Allocate(std::size_t size);

		/// Return a pointer to the memory pool. 
		/// @param[in]  pBlock - block of memory deallocate (i.e push onto free-list)
		void Deallocate(void* pBlock);

		/// Get the allocator name string.
		/// @return		A pointer to the allocator name or NULL if none was assigned.
		const char* GetName() const { return m_name; }

		/// Gets the fixed block memory size, in bytes, handled by the allocator.
		/// @return		The fixed block size in bytes.
		std::size_t GetBlockSize() const { return m_blockSize; }

		/// Gets the maximum number of blocks created by the allocator.
		/// @return		The number of fixed memory blocks created.
		std::size_t GetBlockCount() const { return m_blockCnt; }

		/// Gets the number of blocks in use.
		/// @return		The number of blocks in use by the application.
		std::size_t GetBlocksInUse() const { return m_blocksInUse; }

		/// Gets the total number of allocations for this allocator instance.
		/// @return		The total number of allocations.
		std::size_t GetAllocations() const { return m_allocations; }

		/// Gets the total number of deallocations for this allocator instance.
		/// @return		The total number of deallocations.
		std::size_t GetDeallocations() const { return m_deallocations; }

	private:
		/// Push a memory block onto head of free-list.
		/// @param[in]  pMemory - block of memory to push onto free-list
		void Push(void* pMemory);

		/// Pop a memory block from head of free-list.
		/// @return     Returns pointer to the block. Otherwise NULL if unsuccessful.
		void* Pop();

		struct Block
		{
			Block* pNext;
		};

		enum AllocatorMode { HEAP_BLOCKS, HEAP_POOL, STATIC_POOL };

		const std::size_t m_blockSize;
		const std::size_t m_objectSize;
		const std::size_t m_maxObjects;
		AllocatorMode m_allocatorMode;
		Block* m_pHead;
		char* m_pPool;
		std::size_t m_poolIndex;
		std::size_t m_blockCnt;
		std::size_t m_blocksInUse;
		std::size_t m_allocations;
		std::size_t m_deallocations;
		const char* m_name;
	};

	// Template class to create external memory pool
	template <class T, std::size_t Objects>
	class AllocatorPool : public Allocator
	{
	public:
		AllocatorPool() : Allocator(sizeof(T), Objects, m_memory)
		{
		}
	private:
		char m_memory[sizeof(T) * Objects];
	};
}


// macro to provide header file interface
#define DECLARE_ALLOCATOR \
    public: \
        void* operator new(std::size_t size) \
		{ \
            return s_allocator.Allocate(size); \
        } \
        void operator delete(void* pObject) \
		{ \
            s_allocator.Deallocate(pObject); \
        } \
    private: \
        static Common::Allocator s_allocator; 

// macro to provide source file interface
#define IMPLEMENT_ALLOCATOR(class, objects, memory) \
	Common::Allocator class::s_allocator(sizeof(class), objects, memory, #class);
