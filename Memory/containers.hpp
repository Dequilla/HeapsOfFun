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

		Point() = default;

		Point(const Point<Type>& point)
			: x(Type(point.x)), y(Type(point.y)) {}

		Point(Type x, Type y)
			: x(Type(x)), y(Type(y)) {}
	};

	template<typename Type, size_t size_width, size_t size_height>
	class Array2D
	{
		Type m_array[size_width * size_height];

		size_t getIndex(size_t x, size_t y) const;
		size_t getIndex(const Point<size_t>& p) const;

	public:
		Type& set(Point<size_t> pos, Type t);
		Type& get(Point<size_t> pos);
		const Type& get(Point<size_t> pos) const;

	};

	template<typename Type, size_t size_width, size_t size_height>
	inline size_t Array2D<Type, size_width, size_height>::getIndex(size_t x, size_t y) const
	{
		assert(x >= 0 && y >= 0 && x < size_width && y < size_height);
		return (x + (y * size_width));
	}

	template<typename Type, size_t size_width, size_t size_height>
	inline size_t Array2D<Type, size_width, size_height>::getIndex(const Point<size_t>& p) const
	{
		assert(p.x >= 0 && p.y >= 0 && p.x < size_width && p.y < size_height);
		return (p.x + (p.y * size_width));
	}

	template<typename Type, size_t size_width, size_t size_height>
	inline Type& Array2D<Type, size_width, size_height>::set(Point<size_t> pos, Type t)
	{
		size_t index = getIndex(pos);
		auto& rT = m_array[index] = Type(t);
		return rT;
	}

	template<typename Type, size_t size_width, size_t size_height>
	inline Type& Array2D<Type, size_width, size_height>::get(Point<size_t> pos)
	{
		size_t index = getIndex(pos);
		return m_array[index];
	}

	template<typename Type, size_t size_width, size_t size_height>
	inline const Type& Array2D<Type, size_width, size_height>::get(Point<size_t> pos) const
	{
		size_t index = getIndex(pos);
		return m_array[index];
	}

}