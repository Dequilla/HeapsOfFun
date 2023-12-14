#pragma once

#include <functional>
#include <cstdlib>

namespace memory 
{
	extern std::function<void*(size_t)> g_allocate;
	extern std::function<void(void*)> g_free;

	inline void set_allocation_methods(std::function<void*(size_t)> allocate, std::function<void(void*)> free)
	{
		g_allocate = allocate;
		g_free = free;
	}

	inline void* mem_allocate(size_t size)
	{
		return g_allocate(size);
	}

	inline void mem_free(void* ptr)
	{
		g_free(ptr);
	}

	// Aligns a memory amount to the most optamized size
	inline size_t align(size_t len) {
		return (len + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);
	}

	class Block
	{
	public:
		size_t size = 0;
		bool active = false;
		void* data = nullptr;
		Block* next = nullptr;
	};

	class SimpleHeap
	{
		void* m_heap = nullptr;
		void* m_currentEnd = nullptr;

		size_t m_largestAvailable = 0;
		void* m_largestAvailablePtr = nullptr;

		size_t m_size = 0;
		size_t m_occupied = 0;

		Block* m_rootBlock = nullptr;

		Block* findInactiveBlock(size_t len);

	public:
		bool reserve(size_t len);
		void quit();

		void* allocate(size_t len);
		void deallocate(void* ptr);
	};

}