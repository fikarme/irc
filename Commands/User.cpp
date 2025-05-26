#include "../Server.hpp"

void Server::user(Client *client, const vector<string> &params) {
	if (!client->getIsAuth() || client->getNickn().empty()) {
		sendInfo(client);
		return;
	}
    if (client->getIsConnected()) {
        sendMsg(client->getFd(), ":ircserv 462 * :Already registered\r\n");
        return;
    }
    if (params.size() < 4) {
        sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
        sendMsg(client->getFd(), "Usage: `USER <username> 0 * <realname>`\r\n");
        return;
    }

    string userN = params[0];
    string hostN = params[1];
    string serverN = params[2];
    string realN = params[3];

    if (userN.find_first_of(" ,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 461 * :Invalid username\r\n");
        return;
    }
    if (hostN != "0") {
        sendMsg(client->getFd(), ":ircserv 461 * :Hostname must be '0'\r\n");
        return;
    }
    if (serverN != "*" && serverN != "localhost") {
        sendMsg(client->getFd(), ":ircserv 461 * :Servername must be '*'\r\n");
        return;
    }
    if (params.size() > 4) {
        if (realN[0] != ':') {
            sendMsg(client->getFd(), ":ircserv 461 * :Invalid realname add ':' at the beginning\r\n");
            return;
        }
        for (size_t i = 4; i < params.size(); i++)
            realN += " " + params[i];
    }
    if (realN[0] == ':')
        realN.erase(0, 1);
    if (realN.find_first_of(":,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 461 * :Invalid realname\r\n");
        return;
    }

    client->setUsern(userN);
    client->setHostn(hostN);
    client->setRealn(realN);
    client->setServern(serverN);
    client->setIsConnected(true);
	sendMsg(client->getFd(),":" + client->getIp() + " 001 " + client->getNickn() + " :Welcome to the Internet Relay Network " + client->getNickn() + "!" + client->getUsern() + "@" + client->getIp() + "\r\n");

    return;
}
