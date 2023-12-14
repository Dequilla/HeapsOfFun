#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <cassert>

#include "timer.hpp"

namespace performence
{
	using namespace std::chrono_literals;

	struct Result
	{
		std::string name;
		timer::Duration average = 0ns;
		timer::Duration total = 0ns;
		std::vector<timer::Duration> runs;

		void print(std::ostream& stream);
	};

	using Callback = void(*)();
	inline Result run_perf_test(const std::string& name, Callback callback, size_t count, size_t record_nth = 1, std::optional<std::ostream*> opt_stream = std::nullopt)
	{
		assert(record_nth >= 1);

		if(opt_stream.has_value())
			*opt_stream.value() << "--- Progress for [" << name << "] tests ---\n[ 0 / " << count << " ]" << std::endl;

		Result results;
		results.name = name;
		timer::Timer timer;

		for(size_t index = 0; index < count; index++)
		{
			timer.start();
			callback();
			timer::Duration dur = timer.now();

			results.average = (results.average + dur) / 2;
			results.total += dur;
			if(record_nth == 1 || index % record_nth == 0)
				results.runs.push_back(dur);

			if(opt_stream.has_value())
				*opt_stream.value()  << "[ " << index + 1 << " / " << count << " ]" << std::endl;
		}

		return results;
	}

}