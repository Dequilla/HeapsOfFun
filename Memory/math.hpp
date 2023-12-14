#pragma once

#include <cmath>
#include <complex>

#include "containers.hpp"

namespace math
{
	
	template<typename Type>
	Type map_to_bounds(Type to_map, containers::Point<Type> from, containers::Point<Type> to)
	{
		return to.x + (((to.y - to.x) / (from.y - from.x)) * (to_map - from.x));
	}

	template<size_t width, size_t height>
	containers::Array2D<double, width, height>* create_mandelbrot(double epsilon = 0.0001, size_t max_iterations = 10)
	{
		double x = 0, y = 0;
		std::complex<double> c, z;
		containers::Array2D<double, width, height>* display = (containers::Array2D<double, width, height>*)memory::mem_allocate(sizeof(containers::Array2D<double, width, height>));

		size_t iterations = 0;
		for(x = -2; x <= 2; x += epsilon)
		{
			for(y = -2; y <= 2; y += epsilon)
			{
				iterations = 0;
				c = std::complex<double>(x, y);
				z = std::complex<double>(0, 0);
				while(std::abs(z) < 2 && iterations < max_iterations)
				{
					z = z*z + c;
					iterations++;
				}
				
				double output = static_cast<double>(iterations % 256) / (double)max_iterations;

				containers::Point<size_t> mapped_coords;
				mapped_coords.x = static_cast<size_t>( map_to_bounds(x, { -2.0, 2.0 }, { 0.0, width} ) );
				mapped_coords.y = static_cast<size_t>( map_to_bounds(y, { -2.0, 2.0 }, { 0.0, height} ) );

				display->set(
					mapped_coords,
					output
				);
			}
		}

		return display;
	}

}