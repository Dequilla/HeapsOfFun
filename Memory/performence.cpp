#include "performence.hpp"

namespace performence 
{

	void Result::print(std::ostream& stream)
	{
		stream << "[RESULT][" << name << "][Average: " << timer::to_milliseconds(average) << "][Total: " << timer::to_milliseconds(total) << "]" << std::endl;
		for(auto& duration : runs)
		{
				stream << "\t[RUN][Duration: " << timer::to_milliseconds(duration) << "]" << std::endl;
		}
	}

}
