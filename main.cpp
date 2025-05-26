#include "Server.hpp"

int main(int ac, char **av) {
	if (ac != 3 || !av[1] || !av[2]) {
		coloredError(RED, "ERROR: Wrong Usage!");
		coloredError(GREEN,"Try this: " + string(av[0]) + " <port> <password>");
		return EXIT_FAILURE;
	}
	if (!Server::isValidPort(av[1])) {
		coloredError(RED, "ERROR: Invalid port number!");
		coloredError(GREEN, "Try this: Use a value between 1 and 65535.");
		return EXIT_FAILURE;
	}
	try
	{
		Server server(av[1], av[2]);
		server.loop();
	}
	catch (const std::exception &e)
	{
		cerr << "Exception: " << e.what() << endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		cerr << "Unknown exception occurred." << endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
