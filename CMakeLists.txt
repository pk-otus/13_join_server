cmake_minimum_required(VERSION 3.2)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
   project(join_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
   project(join_server VERSION 0.0.1)
endif()

find_package(Boost COMPONENTS system REQUIRED)
find_package(GTest REQUIRED)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB HEADER_FILES ${INCLUDE_DIR}/*.h)
add_executable(join_server ${HEADER_FILES} main.cpp)

add_executable(join_server_tests ${HEADER_FILES} test/test_main.cpp)

set_target_properties(join_server_tests PROPERTIES INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS} )

set_target_properties(join_server PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
	INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR};${INCLUDE_DIR}"
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set_target_properties(join_server PROPERTIES COMPILE_OPTIONS -W3)
	target_link_libraries(join_server ${Boost_LIBRARIES} )
	target_link_libraries(join_server_tests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
else()
	set_target_properties(join_server PROPERTIES COMPILE_OPTIONS -Wpedantic -Wall -Wextra)
	target_link_libraries(join_server pthread ${Boost_LIBRARIES} )
	target_link_libraries(join_server_tests pthread ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
endif()

enable_testing()
add_test(test_symmetric_difference	join_server_tests)
add_test(test_intersection			join_server_tests)
add_test(test_output				join_server_tests)
add_test(test_commands				join_server_tests)
add_test(test_command_errors		join_server_tests)

install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT a@a.a)

include (CPack)

