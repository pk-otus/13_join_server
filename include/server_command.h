#pragma once
#include <memory>
#include <string>

#include "command_handler.h"

namespace join_server
{
	template<size_t MAX_INDEX> class command_handler;
	
	class abstract_command
	{
	public:		
		using handler_t = command_handler<2>;

		static std::unique_ptr<abstract_command> CreateCommand(const std::string& s);

		virtual ~abstract_command() = default;
		virtual std::string perform(handler_t& server) const = 0;
	};

	template<size_t INDEX>
	class insert_command : public abstract_command
	{
	public:
		insert_command(int k, const std::string& v) : key(k), value(v) {}

		std::string perform(handler_t& server) const override
		{
			return server.insert<INDEX>(key, value);
		}
	private:
		int key;
		std::string value;
	};

	template<size_t INDEX>
	class truncate_command : public abstract_command
	{
	public:
		std::string perform(handler_t& server) const override
		{
			return server.truncate<INDEX>();
		}
	};
	template<size_t I1, size_t I2>
	class symmetric_difference_command : public abstract_command
	{
	public:
		std::string perform(handler_t& server) const override
		{
			return server.symmetric_difference<I1, I2>();
		}
	};

	template<size_t I1, size_t I2>
	class intersection_command : public abstract_command
	{
	public:
		std::string perform(handler_t& server) const override
		{
			return server.intersection<I1, I2>();
		}
	};

	inline std::unique_ptr<abstract_command> abstract_command::CreateCommand(const std::string& strCmd)
	{
		auto pos_cmd_end = strCmd.find(' ');
		std::string cmd = strCmd.substr(0, pos_cmd_end);
		if (std::string::npos == pos_cmd_end)
		{
			if ("INTERSECTION" == cmd)
			{
				return std::unique_ptr<abstract_command>(new intersection_command<0, 1>());
			}
			if ("SYMMETRIC_DIFFERENCE" == cmd)
			{
				return std::unique_ptr<abstract_command>(new symmetric_difference_command<0, 1>());
			}
		}
		else
		{
			auto pos_table_end = strCmd.find(' ', pos_cmd_end + 1);
			std::string table = strCmd.substr(pos_cmd_end + 1, pos_table_end - pos_cmd_end - 1);			
			if (std::string::npos == pos_table_end)
			{
				if ("TRUNCATE" == cmd)
				{
					if ("A" == table)
					{
						return std::unique_ptr<abstract_command>(new truncate_command<0>());
					}
					if ("B" == table)
					{
						return std::unique_ptr<abstract_command>(new truncate_command<1>());
					}
				}
			}
			else
			{
				if ("INSERT" == cmd)
				{
					auto pos_key_end = strCmd.find(' ', pos_table_end + 1);					
					if (std::string::npos != pos_key_end)
					{
						int key = std::stoi(strCmd.substr(pos_table_end + 1, pos_key_end - pos_table_end - 1));
						std::string val = strCmd.substr(pos_key_end + 1);
						if ("A" == table)
						{
							return std::unique_ptr<abstract_command>(new insert_command<0>(key, val));
						}
						if ("B" == table)
						{
							return std::unique_ptr<abstract_command>(new insert_command<1>(key, val));
						}
					}
				}
			}			
		}

		return nullptr;
	}
}
