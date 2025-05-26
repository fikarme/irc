#include "../Server.hpp"

void Server::part(Server *server, Client *client, const vector<string> &params) {
    if (!client->getIsConnected()) {
        sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
        sendInfo(client);
        return;
    }
    if (params.size() < 1) {
        sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
        sendMsg(client->getFd(), "Usage: `PART <channel> [<message>]`\r\n");
        return;
    }
    string channelName = params[0];
    string message = "";
    if (params.size() > 1) {
        message = params[1];
        if (message[0] == ':')
            message.erase(0, 1);
        for (size_t i = 2; i < params.size(); i++)
            message += " " + params[i];
        if (message.find_first_of(",\a") != string::npos) {
            sendMsg(client->getFd(), ":ircserv 461 * :Invalid message\r\n");
            return;
        }
    }
    for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
        if (it->getName() == channelName) {
            if (!it->isInChannel(client->getNickn())) {
                sendMsg(client->getFd(), ":ircserv 442 * :You're not on that channel\r\n");
                return;
            }
            if (it->isOp(*client) && it->getClients().size() > 1 && it->getOps().size() == 1) {
                sendMsg(client->getFd(), ":ircserv 400 * :Make someone else operator\r\n");
                return;
            }
            it->removeClient(*client);
            it->broadcastMsg(clientPrefix(client) + " PART " + channelName + " :" + message + "\r\n");
            if (it->getClients().empty())
                server->deleteChannel(channelName);
            return;
        }
    }
    sendMsg(client->getFd(), ":ircserv 403 * :No such channel\r\n");
}
