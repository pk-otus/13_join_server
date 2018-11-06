#include "include/server.h"

int main(int argc, char const *argv[]) 
{
	if (argc < 2)
	{
		std::cout << "Usage: join_server <server port>\n";
		return 1;
	}
	size_t port = std::stoi(argv[1]);
	
	join_server::server q(port);

	while (true) 
	{
		q.AcceptConnection();
	}

	return 0;
}
