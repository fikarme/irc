#include "../Server.hpp"

void Server::quit(Server *server, Client *client, const vector<string> &params) {
    string reason = "";
    if (params.size() > 0) {
        reason = params[0];
        if (reason[0] == ':') {
            reason.erase(0, 1);
            for (size_t i = 1; i < params.size(); i++)
                reason += " " + params[i];
        }
        else if (params.size() > 1) {
            sendMsg(client->getFd(), ":ircserv 461 * :Invalid reason add ':' at the beginning\r\n");
            return;
        }
        if (reason.find_first_of(",\a") != string::npos) {
            sendMsg(client->getFd(), ":ircserv 400 * :Invalid reason\r\n");
            return;
        }
    }
    vector<string> channelsToDelete;
    for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it)
        if (it->isInChannel(client->getNickn())) {
            if (it->isOp(*client) && it->getOps().size() == 1) {
                it->broadcastMsg("ircserv 400 * :Channel got orphaned\r\n");
            }
            it->removeClient(*client);
            it->broadcastMsg(clientPrefix(client) + " QUIT :" + reason + "\r\n");
            if (it->getClients().empty())
                channelsToDelete.push_back(it->getName());
        }
    vector<string>::iterator nameIt = channelsToDelete.begin();
    for (; nameIt != channelsToDelete.end(); ++nameIt) {
        server->deleteChannel(*nameIt);
    }
    sendMsg(client->getFd(), ":ircserv 001 * :Goodbye!\r\n");
    server->removeClient(client);
}
