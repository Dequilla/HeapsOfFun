#pragma once

#include <cstdlib>
#include <ctime>

#include <vector>
#include <random>

namespace random
{
	std::default_random_engine g_random_engine;

	inline void init()
	{
		std::random_device rd;
		g_random_engine = std::default_random_engine(rd());
	}

	inline std::vector<uint32_t> random_sequence(size_t count, uint32_t min = 0, uint32_t max = 100)
	{
		std::uniform_int_distribution<uint32_t> uni(min, max);

		std::vector<uint32_t> nums;
		for(size_t index = 0; index < count; index++)
		{
			uint32_t num = uni(g_random_engine);
			nums.push_back(num);
		}
		return nums;
	}

}