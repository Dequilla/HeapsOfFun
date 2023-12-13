#pragma once

#include <cmath>

#include "containers.hpp"

namespace math
{

	template<typename Type>
	class Complex
	{
	public:
		Type a; // real
		Type b; // imaginary

		template<typename Type>
		Type abs()
		{
			return std::sqrt( (a * a) + (b * b) );
		}

		template<typename Type>
		friend Complex<Type> operator+(Complex<Type> left, const Complex<Type>& right)
		{
			left.a += right.a;
			left.b += right.b;
			return left;
		}

		template<typename Type>
		friend Complex<Type> operator-(Complex<Type> left, const Complex<Type>& right)
		{
			left.a -= right.a;
			left.b -= right.b;
			return left;
		}

		template<typename Type>
		friend Complex<Type> operator*(Complex<Type> left, const Complex<Type>& right)
		{
			// zw = (a + ib) (c + id)
			// (ac - bd) + i(ad + bc)
			left.a = ((left.a * right.a) - (left.b * right.b));
			left.b = ((left.a * right.b) + (left.b * right.a));

			return left;
		}

		template<typename Type>
		friend Complex<Type> operator/(Complex<Type> left, const Complex<Type>& right)
		{
			// (ac + bd)     (bc - ad)
			// --------- + i --------- 
			// c^2 + d^2     c^2 + d^2

			Type c = (right.a * right.a) + (right.b * right.b);

			left.a = ((left.a * right.a) + (left.b * right.b)) / c;
			left.b = ((left.b * right.a) - (left.a * right.b)) / c;

			return left;
		}
	};


}