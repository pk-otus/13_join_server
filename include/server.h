#pragma once
#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <boost/asio.hpp>

#include "server_command.h"
#include "command_handler.h"

namespace join_server
{
	namespace ba = boost::asio;

	class server
	{
	public:
		server(uint16_t port) :
			acc(service, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port))
		{
			std::thread(&server::consumer_thread, this).detach();
		}

		~server() = default;

		void AcceptConnection()
		{
			auto sock = std::make_shared<ba::ip::tcp::socket>(service);
			acc.accept(*sock);
			std::thread(&server::producer_thread, this, sock).detach();
		}

	private:
		void consumer_thread()
		{
			while(true)
			{						
				try 
				{
					std::unique_lock<std::mutex> lock(guard_mutex);
					cond_var.wait(lock,	[this]() { return !q.empty(); });

					auto cmd = std::move(q.front());
					q.pop();
					lock.unlock();
					if (auto psock = cmd.first.lock())
					{
						auto result = cmd.second->perform(handler) + '\n';
						write(*psock, ba::buffer(result, result.size()));
					}
				}
				catch (const std::exception &e) 
				{
					std::cout << "consumer thread error: " << e.what() << '\n';
					break;
				}

			}
		}

		void producer_thread(std::shared_ptr<ba::ip::tcp::socket> sock)
		{			
			while (true)
			{
				try {
					ba::streambuf buf;
					read_until(*sock, buf, '\n');

					auto str = std::string(	std::istreambuf_iterator<char>(&buf),
											std::istreambuf_iterator<char>());

					str.erase(std::remove_if(str.begin(), str.end(),
						[](char chr) { return chr == '\r'; }), str.end());

					size_t pos_end;					
					do
					{
						pos_end = str.find('\n');
						auto str_cmd = str.substr(0, pos_end);
						str = str.substr(pos_end + 1);
												
						if (auto cmd = abstract_command::CreateCommand(str_cmd))
						{
							{
								std::lock_guard<std::mutex> lock(guard_mutex);
								q.push(std::make_pair(sock, std::move(cmd)));
							}
							cond_var.notify_one();
						}
						else
						{
							auto result = error_string(error_code::parse_error) + '\n';
							write(*sock, ba::buffer(result, result.size()));
						}
					} 
					while (std::string::npos != pos_end && !str.empty());
				}
				catch (const std::exception &e) {
					std::cout << "producer thread error: " << e.what() << '\n';
					break;
				}
			}
		}

		ba::io_service			service;
		ba::ip::tcp::acceptor	acc;

		abstract_command::handler_t handler;
		std::mutex					guard_mutex;
		std::condition_variable		cond_var;
		std::queue<std::pair<	std::weak_ptr<ba::ip::tcp::socket>,
								std::unique_ptr<abstract_command>>> q;
		
	};
}
