#include "../Server.hpp"

void Server::cap(Client * client, const vector<string>& params) {
    if (!params.empty())
   		if (params[0] == "LS" && params.size() == 1 && !client->getIsConnected())
			return;

	if (!client->getIsConnected()) {
		sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
		sendInfo(client);
		return;
	}
	sendMsg(client->getFd(), ":ircserv 421 * :Unknown command\r\n");
	return;
}
