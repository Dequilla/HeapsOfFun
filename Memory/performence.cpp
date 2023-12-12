#include "performence.hpp"

namespace performence 
{

	void Result::print(std::ostream& stream)
	{
		stream << "[RESULT][" << name << "][Average: " << timer::to_milliseconds(average) << "]" << std::endl;
		for(auto& duration : runs)
		{
				stream << "\t[RUN][Duration: " << timer::to_milliseconds(duration) << "]" << std::endl;
		}
	}

}
