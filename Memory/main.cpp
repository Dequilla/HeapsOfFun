#include "memory.hpp"
#include "timer.hpp"
#include "random.hpp"
#include "performence.hpp"
#include "containers.hpp"
#include "math.hpp"
#include "visual.hpp"

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

std::vector<uint32_t> g_sequence;

struct Point
{
	uint32_t x;
	uint32_t y;
};

void test_malloc()
{
	for(uint32_t num : g_sequence)
	{
		Point* p = (Point*)malloc(num * sizeof(Point));
		for(size_t index = 0; index < num; index++)
		{
			Point* p2 = &p[index];
			p2->x = num / 2;
			p2->y = num / 3;
		}
		free(p);
	}
}

void test_new_cpp()
{
	for(uint32_t num : g_sequence)
	{
		Point* p = new Point[num];
		for(size_t index = 0; index < num; index++)
		{
			Point* p2 = &p[index];
			p2->x = num / 2;
			p2->y = num / 3;
		}
		delete[] p;
	}
}

memory::SimpleHeap g_sheap;
void test_simple_heap()
{
	for(uint32_t num : g_sequence)
	{
		Point* p = (Point*)g_sheap.allocate(num * sizeof(Point));
		for(size_t index = 0; index < num; index++)
		{
			Point* p2 = &p[index];
			p2->x = num / 2;
			p2->y = num / 3;
		}
		g_sheap.deallocate(p);
	}
}

int main(int argc, char** argv)
{
	////////////////////////////////////////////
	// Just a heads up that the tests are not 
	// overly well optimized to actually test
	// the heaps properly. This is a very
	// naive approach to permormence testing.
	// PS: Also naive approach to heaps.
	
	random::init();
	g_sequence = random::random_sequence(10000);
	g_sheap.reserve(1000000000); // 1gb

	const size_t TEST_COUNT = 500;
	const size_t TEST_RECORD_NTH = 50;



	


	/*

	performence::Result res_malloc = performence::run_perf_test("malloc", &test_malloc, TEST_COUNT, TEST_RECORD_NTH);
	performence::Result res_new = performence::run_perf_test("new", &test_new_cpp, TEST_COUNT, TEST_RECORD_NTH);
	performence::Result res_simple_heap = performence::run_perf_test("SimpleHeap", &test_simple_heap, TEST_COUNT, TEST_RECORD_NTH);

	res_malloc.print(std::cout);
	res_new.print(std::cout);
	res_simple_heap.print(std::cout);

	*/


	const size_t WIDTH = 1280;
	const size_t HEIGHT = 720;
	
	auto display = math::create_mandelbrot<WIDTH, HEIGHT>(0.0005, 50);
	visual::terminal::draw_pixels(*display, { 10, 20 }, visual::ColorRGB { .red = 1.0, .green = 0.0, .blue = 0.0 });
	memory::mem_free((void*)display);
	std::cin.get();


	g_sheap.quit();
	return 0;
}