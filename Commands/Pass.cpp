#include "../Server.hpp"

void Server::pass(Server *server, Client *client, const vector<string> &params) {
    if (params.empty()) {
		sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
		sendMsg(client->getFd(), "Usage: `PASS <password>`\r\n");
		return;
	}
	
	if (params.size() > 1) {
		sendMsg(client->getFd(), ":ircserv 461 * :Too many parameters\r\n");
		sendMsg(client->getFd(), "Usage: `PASS <password>`\r\n");
		return;
	}

    if (client->getIsAuth()) {
        sendMsg(client->getFd(), ":ircserv 462 * :Already registered\r\n");
        return;
    }
    
    string password = params[0];
    
    if (password == server->getServerPass()) {
        client->setIsAuth(true);
        sendMsg(client->getFd(), ":ircserv 001 * :Password accepted\r\n");
    } else {
        sendMsg(client->getFd(), ":ircserv 464 * :Password mismatch\r\n");
    }
}
