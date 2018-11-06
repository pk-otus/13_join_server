#pragma once
#include "table.h"
#include "error_code.h"

namespace join_server
{
	template<size_t MAX_INDEX>
	class command_handler
	{
	public:
		template<size_t I1>
		std::string insert(int key, const std::string& val)
		{
			static_assert(I1 < MAX_INDEX, "max index fail");

			return tables[I1].insert(key, val) 
					? error_string(error_code::ok)
					: error_string(error_code::duplicate) + ' ' + std::to_string(key);
		}
		
		template<size_t I1>
		std::string truncate()
		{
			static_assert(I1 < MAX_INDEX, "max index fail");

			tables[I1].truncate();
			return error_string(error_code::ok);
		}
		template<size_t I1, size_t I2>
		std::string symmetric_difference() const
		{
			static_assert(I1 < MAX_INDEX, "max index fail");
			static_assert(I2 < MAX_INDEX, "max index fail");

			return	table::symmetric_difference(tables[I1], tables[I2]).to_string() +
					error_string(error_code::ok);
		}

		template<size_t I1, size_t I2>
		std::string intersection() const
		{
			static_assert(I1 < MAX_INDEX, "max index fail");
			static_assert(I2 < MAX_INDEX, "max index fail");

			return	table::intersection(tables[I1], tables[I2]).to_string() +
					error_string(error_code::ok);
		}

	private:
		table tables[MAX_INDEX];
	};
}
