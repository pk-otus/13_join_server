#pragma once
#include <string>
#include <map>

namespace join_server
{
	struct result_table
	{
		std::string to_string() const
		{
			std::string result;
			for (const auto& item : data)
			{
				result += std::to_string(item.first) + ',' +
					item.second.first + ',' +
					item.second.second + '\n';
			}
			return result;
		}

		std::map<int, std::pair<std::string, std::string>> data;
	};

	class table
	{
	public:
		static result_table intersection(const table&left, const table& right) noexcept
		{
			result_table result;
			auto il = left.data.begin();
			auto ir = right.data.begin();

			while (il != left.data.end() && ir != right.data.end())
			{
				if (il->first < ir->first)
					++il;
				else if (ir->first < il->first)
					++ir;
				else
				{
					result.data.insert(make_pair(il->first, make_pair(il->second, ir->second)));
					++il;
					++ir;
				}
			}
			return result;
		}

		static result_table symmetric_difference(const table&left, const table& right) noexcept
		{
			result_table result;

			auto il = left.data.begin();
			auto ir = right.data.begin();

			while (il != left.data.end() || ir != right.data.end())
			{
				if (ir == right.data.end() ||
					il != left.data.end() && il->first < ir->first)
				{
					result.data.insert(make_pair(il->first, make_pair(il->second, "")));
					++il;
				}
				else if (il == left.data.end() ||
						ir != right.data.end() && ir->first < il->first)
				{
					result.data.insert(make_pair(ir->first, make_pair("", ir->second)));
					++ir;
				}
				else
				{					
					++il;
					++ir;
				}
			}
			return result;
		}

		bool insert(int id, const std::string& name) noexcept
		{
			return data.insert({ id, name }).second;
		}

		void truncate() noexcept
		{
			data.clear();
		}
	private:
		std::map<int, std::string> data;
	};
}
