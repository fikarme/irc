#include "../Server.hpp"

void Server::nick(Server *server, Client *client, const vector<string> &params) {
    if (!client->getIsAuth()) {
        sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
		sendInfo(client);
        return;
    }
    if (params.size() == 0) {
        sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
        sendMsg(client->getFd(), "Usage: `NICK <nickname>`\r\n");
        return;
    }
    if (params.size() > 1) {
        sendMsg(client->getFd(), ":ircserv 461 * :Too many parameters\r\n");
        sendMsg(client->getFd(), "Usage: `NICK <nickname>`\r\n");
        return;
    }

    string newNick = params[0];
    
    
    if (newNick.empty()) {
        sendMsg(client->getFd(), ":ircserv 431 * :No nickname given\r\n");
        return;
    }
    if (newNick.length() > 9) {
        sendMsg(client->getFd(), ":ircserv 432 * :Nickname too long\r\n");
        return;
    }
    if (newNick.find_first_of(" :#\r\n") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 432 * :Nickname contains invalid characters\r\n");
        return;
    }
    if (client->getNickn() == newNick) {
        sendMsg(client->getFd(), ":ircserv 433 * :Why would u do that?\r\n");
        return;
    }
	const std::map<int, Client*>& clients = server->getOurClients();
	for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second && it->second->getNickn() == newNick) {
            sendMsg(client->getFd(), ":ircserv 433 " + client->getNickn() + " :Nickname is already in use\r\n");
            return;
        }
    }

    string oldNick = client->getNickn();
    client->setNickn(newNick);
    server->updateServerWideNickN(newNick, client->getFd());
    sendMsg(client->getFd(), ":ircserv 001 * :Nickname set to -> " + newNick + "\r\n");
    if (!oldNick.empty()) {
        for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
            if (it->isInChannel(oldNick)) {
                it->updateNickn(oldNick, newNick);
                it->broadcastMsg(":" + oldNick + "!" + client->getUsern() + "@" + client->getHostn() + " NICK :" + newNick + "\r\n");
            }
        }
    }
}
