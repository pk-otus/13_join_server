#include <gtest/gtest.h>

#include "../include/command_handler.h"
#include "../include/server_command.h"
#include "../include/table.h"

using namespace join_server;

namespace
{
	struct test_tables
	{
		test_tables()
		{
			A.insert(0, "lean");
			A.insert(1, "sweater");
			A.insert(2, "frank");
			A.insert(3, "violation");
			A.insert(4, "quality");
			A.insert(5, "precision");

			B.insert(3, "proposal");
			B.insert(4, "example");
			B.insert(5, "lake");
			B.insert(6, "flour");
			B.insert(7, "wonder");
			B.insert(8, "selection");
		}

		table A;
		table B;
	};
	result_table intersection_result =
	{
		{
			{3 , {"violation"	, "proposal"	}},
			{4 , {"quality"		, "example"		}},
			{5 , {"precision"	, "lake"		}}
		}
	};

	result_table symmetric_difference_result
	{
		{
			{0 , {"lean"	, ""			}},
			{1 , {"sweater"	, ""			}},
			{2 , {"frank"	, ""			}},
			{6 , {""		, "flour"		}},
			{7 , {""		, "wonder"		}},
			{8 , {""		, "selection"	}}
		}
	};

	std::string intersection_control = 
		"3,violation,proposal\n4,quality,example\n5,precision,lake\n";
	std::string symmetric_difference_control = 
		"0,lean,\n1,sweater,\n2,frank,\n6,,flour\n7,,wonder\n8,,selection\n";

	bool command_success(abstract_command::handler_t& h, const std::string& str)
	{
		return	error_string(error_code::ok) == abstract_command::CreateCommand(str)->perform(h);
	}
}
TEST(join_server_tests, test_symmetric_difference)
{
	test_tables tables;
	
	ASSERT_EQ(table::symmetric_difference(tables.A, tables.B).data,	symmetric_difference_result.data);
}

TEST(join_server_tests, test_intersection)
{
	test_tables tables;
	
	ASSERT_EQ(table::intersection(tables.A, tables.B).data,	intersection_result.data);
}

TEST(join_server_tests, test_output)
{
	test_tables tables;
	
	ASSERT_EQ(table::intersection(tables.A, tables.B).to_string(), intersection_control);

}

TEST(join_server_tests, test_commands)
{	
	abstract_command::handler_t handler;

	ASSERT_TRUE(command_success(handler, "INSERT A 0 lean"));
	ASSERT_TRUE(command_success(handler, "INSERT A 1 sweater"));
	ASSERT_TRUE(command_success(handler, "INSERT A 2 frank"));
	ASSERT_TRUE(command_success(handler, "INSERT A 3 violation"));
	ASSERT_TRUE(command_success(handler, "INSERT A 4 quality"));
	ASSERT_TRUE(command_success(handler, "INSERT A 5 precision"));

	ASSERT_TRUE(command_success(handler, "INSERT B 3 proposal"));
	ASSERT_TRUE(command_success(handler, "INSERT B 4 example"));
	ASSERT_TRUE(command_success(handler, "INSERT B 5 lake"));
	ASSERT_TRUE(command_success(handler, "INSERT B 6 flour"));
	ASSERT_TRUE(command_success(handler, "INSERT B 7 wonder"));
	ASSERT_TRUE(command_success(handler, "INSERT B 8 selection"));

	ASSERT_EQ(	abstract_command::CreateCommand("INTERSECTION")->perform(handler), 
				intersection_control + error_string(error_code::ok));

	ASSERT_EQ(	abstract_command::CreateCommand("SYMMETRIC_DIFFERENCE")->perform(handler),
				symmetric_difference_control + error_string(error_code::ok));

	ASSERT_TRUE(command_success(handler, "TRUNCATE A"));
	ASSERT_TRUE(command_success(handler, "TRUNCATE B"));

	ASSERT_TRUE(command_success(handler, "INTERSECTION"));
	ASSERT_TRUE(command_success(handler, "SYMMETRIC_DIFFERENCE"));
}

TEST(join_server_tests, test_command_errors)
{
	abstract_command::handler_t handler;

	ASSERT_EQ(abstract_command::CreateCommand("INSERT"), nullptr);
	
	ASSERT_TRUE(command_success(handler, "INSERT A 0 x"));
	
	ASSERT_EQ(abstract_command::CreateCommand("INSERT A 0 y")->perform(handler),
		error_string(error_code::duplicate) + " 0");
}