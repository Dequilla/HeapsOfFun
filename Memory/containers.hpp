#pragma once

#include <cassert>
#include <utility>

namespace containers
{
	template<typename Type>
	struct Point
	{
		Type x;
		Type y;
	};

	template<typename Type, size_t size_width, size_t size_height>
	class Array2D
	{
		Type m_array[size_width * size_height];

		size_t getIndex(size_t x, size_t y);
		size_t getIndex(const Point<Type>& p);

	public:
		Type& set(Point<Type> pos, const Type&& t);
		Type& get(Point<Type> pos);
	};

	template<typename Type, size_t size_width, size_t size_height>
	inline size_t Array2D<Type, size_width, size_height>::getIndex(size_t x, size_t y)
	{
		assert(x >= 0 && y >= 0 && x < size_width && y < size_height);
		return (x + (y * size_width));
	}

	template<typename Type, size_t size_width, size_t size_height>
	inline size_t Array2D<Type, size_width, size_height>::getIndex(const Point<Type>& p)
	{
		assert(p.x >= 0 && p.y >= 0 && p.x < size_width && p.y < size_height);
		return (p.x + (p.y * size_width));
	}

	template<typename Type, size_t size_width, size_t size_height>
	inline Type& Array2D<Type, size_width, size_height>::set(Point<Type> pos, const Type&& t)
	{
		size_t index = getIndex(pos);
		return (m_array[index] = std::move(t));
	}

	template<typename Type, size_t size_width, size_t size_height>
	inline Type& Array2D<Type, size_width, size_height>::get(Point<Type> pos)
	{
		size_t index = getIndex(pos);
		return m_array[index];
	}

}