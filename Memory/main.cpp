#include "memory.hpp"
#include "timer.hpp"
#include "random.hpp"
#include "performence.hpp"

#include <iostream>
#include <vector>

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

void test_simple_heap()
{
	memory::SimpleHeap sheap;
	sheap.reserve(1000000000); // 1gb

	for(uint32_t num : g_sequence)
	{
		Point* p = (Point*)sheap.allocate(num * sizeof(Point));
		for(size_t index = 0; index < num; index++)
		{
			Point* p2 = &p[index];
			p2->x = num / 2;
			p2->y = num / 3;
		}
		sheap.deallocate(p);
	}

	sheap.quit();
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
	
	const size_t TEST_COUNT = 10;

	performence::Result res_malloc = performence::run_perf_test("malloc", &test_malloc, TEST_COUNT, 2);
	performence::Result res_new = performence::run_perf_test("new", &test_new_cpp, TEST_COUNT, 2);
	performence::Result res_simple_heap = performence::run_perf_test("SimpleHeap", &test_simple_heap, TEST_COUNT, 2);
	
	res_malloc.print(std::cout);
	res_new.print(std::cout);
	res_simple_heap.print(std::cout);
	
	return 0;
}