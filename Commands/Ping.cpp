#include "../Server.hpp"

void Server::ping(Client *client, const vector<string> &params) {
	if (params.empty()) {
		sendMsg(client->getFd(), ":ircserv 409 * :No origin specified\r\n");
		sendMsg(client->getFd(), "Usage: `PING <server>`\r\n");
		return;
	}
	if (params.size() == 2)
		return;
	if (params.size() > 2) {
		sendMsg(client->getFd(), ":ircserv 461 * :Too many parameters\r\n");
		sendMsg(client->getFd(), "Usage: `PING <server>`\r\n");
		return;
	}
	string server = params[0];
	if (!params[0].empty() && params[0][0] == ':')
		server.erase(0, 1);
	sendMsg(client->getFd(), ":" + clientPrefix(client) + " PONG " + server + "\r\n");
}
