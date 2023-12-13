#pragma once

#include <cstdlib>
#include <ctime>

#include <vector>

namespace random
{

	inline void init()
	{
		 std::srand(static_cast<unsigned int>(std::time(nullptr)));
	}

	inline std::vector<uint32_t> random_sequence(size_t count)
	{
		std::vector<uint32_t> nums;
		for(size_t index = 0; index < count; index++)
		{
			uint32_t num = std::rand();
			nums.push_back(num);
		}
		return nums;
	}

}