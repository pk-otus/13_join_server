#pragma once
#include <string>

namespace join_server
{
	enum class error_code
	{
		ok			= 0,
		parse_error = 1,
		duplicate	= 2
	};

	inline std::string error_string(error_code ec)
	{
		const char * ERRORS[] = {	"OK",
									"parse error",
									"duplicate element" };

		return	(error_code::ok == ec ? std::string() : std::string("ERR ")) + 
				ERRORS[static_cast<int>(ec)];
	}
}
