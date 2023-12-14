#pragma once

#include "containers.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace visual
{
	struct ColorRGB
	{
		double red = 0;
		double green = 0;
		double blue = 0;
	};

	namespace terminal
	{

		template<size_t width, size_t height>
		void draw_pixels(const containers::Array2D<double, width, height>& display, containers::Point<size_t> offset = { 0, 0 }, ColorRGB color_amounts = { 1.0, 1.0, 1.0 });

	}
}

template<size_t width, size_t height>
void visual::terminal::draw_pixels(const containers::Array2D<double, width, height>& display, containers::Point<size_t> offset, ColorRGB color_amounts)
{
#ifdef _WIN32
	HDC hdc = GetDC(GetConsoleWindow());
	for(size_t y = 0; y < height; y++)
	{
	
		for(size_t x = 0; x < width; x++)
		{
			const double& color_strength = display.get({x, y});
			int color = color_strength * 256.0;
			SetPixel(
				hdc,
				offset.x + x,
				offset.y + y,
				RGB(
					color * color_amounts.red, 
					color * color_amounts.green, 
					color * color_amounts.blue
				)
			);
		}
	
	}
#else
	assert(false && "Not implemented for non windows.");
#endif
}