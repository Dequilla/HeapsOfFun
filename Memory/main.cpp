#include "memory.hpp"
#include "timer.hpp"
#include "random.hpp"
#include "performence.hpp"
#include "containers.hpp"
#include "math.hpp"
#include "visual.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#ifdef _WIN32
#include <Windows.h>
#endif

const uint32_t g_RAND_SEQ_MIN = 0;
const uint32_t g_RAND_SEQ_MAX = 1000;
const size_t g_RAND_SEQ_COUNT = 10;

const size_t g_TEST_COUNT = 5;
const size_t g_TEST_RECORD_NTH = 2;

const size_t g_GIGABYTE = 1000000000;

const size_t g_WIDTH = 1280;
const size_t g_HEIGHT = 720;
const bool g_RUN_VISUALISATION = false;

std::vector<uint32_t> g_sequence;
memory::SimpleHeap g_sheap;

void run_test_mandelbrot()
{
	// Try and make caching difficult.
	std::shuffle(std::begin(g_sequence), std::end(g_sequence), random::g_random_engine);

	for(auto& num : g_sequence)
	{
		double epsilon = 0.5 / num;
		size_t max_iterations = 1 * (num / 50);

		auto display = math::create_mandelbrot<g_WIDTH, g_HEIGHT>(epsilon, max_iterations);
		if(g_RUN_VISUALISATION)
			visual::terminal::draw_pixels(*display, { 0, 0 }, visual::ColorRGB { .red = 1.0, .green = 0.0, .blue = 0.0 });
		memory::mem_free((void*)display);
	}

}

void run_test_only_allocations()
{
	std::shuffle(std::begin(g_sequence), std::end(g_sequence), random::g_random_engine);

	std::vector<char*> texts;
	size_t accumilator = 0;
	for(auto& num : g_sequence)
	{
		size_t total = num * 5;
		char* text = (char*)memory::mem_allocate(sizeof(char) * total);
		texts.push_back(text);
		memset((char*)text, '\0', total * sizeof(char));
		for(size_t index = 0; index < total; index++)
		{
			char* pNC = (char*)memory::mem_allocate(sizeof(char)); // Use more memory to test it more
			*pNC = text[index] & (char)0b10101010; // Do something to not get optimized away or something stupid like that.
			text[index] = *pNC;
			memory::mem_free(pNC);
		}

		accumilator += 1;
		if(accumilator >= 10)
		{
			for(auto ptr : texts)
				memory::mem_free(ptr);
			texts.clear();
			accumilator = 0;
		}
	}

	// If any left clear them
	for(auto ptr : texts)
		memory::mem_free(ptr);
}

void init_globals()
{
	random::init();
	g_sequence = random::random_sequence(g_RAND_SEQ_COUNT, g_RAND_SEQ_MIN, g_RAND_SEQ_MAX);
	g_sheap.reserve(g_GIGABYTE);
}

void quit_globals()
{
	g_sheap.quit();
}

int main(int argc, char** argv)
{
	////////////////////////////////////////////
	// Just a heads up that the tests are not 
	// overly well optimized to actually test
	// the heaps properly. This is a very
	// naive approach to permormence testing.
	// PS: Also naive approach to heaps.

	init_globals();

	//////////////////////////////
	// malloc
	memory::set_allocation_methods(
		[](size_t size) -> void* { return std::malloc(size); },
		[](void* ptr) { return std::free(ptr); }
	);
	performence::Result res_malloc = performence::run_perf_test("malloc MB", &run_test_mandelbrot, g_TEST_COUNT, g_TEST_RECORD_NTH, &std::cout);
	performence::Result res_malloc_allocs = performence::run_perf_test("malloc Allocs", &run_test_only_allocations, g_TEST_COUNT, g_TEST_RECORD_NTH, &std::cout);
	
	//////////////////////////////
	// new
	memory::set_allocation_methods(
		[](size_t size) -> void* { return new uint8_t[size]; },
		[](void* ptr) { return delete[] ptr; }
	);
	performence::Result res_new = performence::run_perf_test("new MB", &run_test_mandelbrot, g_TEST_COUNT, g_TEST_RECORD_NTH, &std::cout);
	performence::Result res_new_allocs = performence::run_perf_test("new Allocs", &run_test_only_allocations, g_TEST_COUNT, g_TEST_RECORD_NTH, &std::cout);

	//////////////////////////////
	// SimpleHeap
	memory::set_allocation_methods(
		[](size_t size) -> void* { return g_sheap.allocate(size); },
		[](void* ptr) { return g_sheap.deallocate(ptr); }
	);
	performence::Result res_simple_heap = performence::run_perf_test("SimpleHeap MB", &run_test_mandelbrot, g_TEST_COUNT, g_TEST_RECORD_NTH, &std::cout);
	performence::Result res_simple_heap_allocs = performence::run_perf_test("SimpleHeap Allocs", &run_test_only_allocations, g_TEST_COUNT, g_TEST_RECORD_NTH, &std::cout);
	
	// Results
	std::cout << "###### Malbrot results ######" << std::endl;
	res_malloc.print(std::cout);
	res_new.print(std::cout);
	res_simple_heap.print(std::cout);
	

	std::cout << "###### Allocation results ######" << std::endl;
	res_malloc_allocs.print(std::cout);
	res_new_allocs.print(std::cout);
	res_simple_heap_allocs.print(std::cout);

	quit_globals();
	return 0;
}