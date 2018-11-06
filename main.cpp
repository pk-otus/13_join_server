#include "include/server.h"

int main(int argc, char const *argv[]) 
{
	join_server::server q(9999);

	while (true) 
	{
		q.AcceptConnection();
	}

	return 0;
}
