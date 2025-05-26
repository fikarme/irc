#include "../Server.hpp"

void Server::kick(Server *server, Client *client, const vector<string> &params) {
    if (!client->getIsConnected()) {
		sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
		sendInfo(client);
		return;
	}
    if (params.size() < 2) {
        sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
        sendMsg(client->getFd(), "Usage: `KICK <channel> <user> [<comment>]`\r\n");
        return;
    }

    string channelName = params[0];
    string userName = params[1];
    string comment = "";

    if (params.size() > 2){
        comment = params[2];
        if (comment[0] == ':') {
            comment.erase(0, 1);
            for (size_t i = 3; i < params.size(); i++)
                comment += " " + params[i];
        }
        else if (params.size() > 3) {
            sendMsg(client->getFd(), ":ircserv 461 * :Invalid comment\r\n");
            return;
        }
    }

    if (channelName.empty() || channelName[0] != '#' || channelName.find_first_of(" ,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 476 * :Invalid channel name\r\n");
        return;
    }
    if (userName.empty() || userName.find_first_of(" ,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 401 * :No such nick\r\n");
        return;
    }
    if (comment.find_first_of(":,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 461 * :Invalid comment\r\n");
        return;
    }

    for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
        if (it->getName() == channelName) {
            if (!it->isInChannel(client->getNickn())) {
                sendMsg(client->getFd(), ":ircserv 442 * :You're not on that channel\r\n");
                return;
            }
            if (!it->isOp(*client)) {
                sendMsg(client->getFd(), ":ircserv 482 * :You're not channel operator\r\n");
                return;
            }
            if (!it->isInChannel(userName)) {
                sendMsg(client->getFd(), ":ircserv 441 * :User not in channel\r\n");
                return;
            }
            if (userName == ":") {
                for (vector<Client>::iterator it2 = it->getClients().begin(); it2 != it->getClients().end(); ++it2) {
                    if (it2->getNickn() != client->getNickn()) {
                        it->broadcastMsg(clientPrefix(client) + " KICK " + channelName + " " + userName + " :" + "genocided" + "\r\n");
                        sendMsg(it2->getFd(), ":ircserv 400 * :KICK " + channelName + " " + userName + " :" + "genocided" + "\r\n");
                        it->removeClient(*it2);
                        it->removeInvited(*it2);
                        it->removeOp(*it2);
                    }
                }
                return;
            }
            if (userName == client->getNickn()) {
                if (it->getOps().size() == 1 && it->getClients().size() > 1) {
                    sendMsg(client->getFd(), ":ircserv 400 * :Make someone else operator at" + it->getName() + "\r\n");
                    return;
                }
                it->broadcastMsg(clientPrefix(client) + " KICK " + channelName + " " + client->getNickn() + " :" + comment + "\r\n");
                sendMsg(client->getFd(), ":ircserv 400 * :KICK " + channelName + " " + client->getNickn() + " :" + comment + "\r\n");
                it->removeClient(*client);
                it->removeInvited(*client);
                it->removeOp(*client);
                if (it->getClients().size() == 0)
                    server->deleteChannel(it->getName());
                return;
            }
            Client userToKick = it->getClientByNick(userName);
            it->broadcastMsg(clientPrefix(client) + " KICK " + channelName + " " + userName + " :" + comment + "\r\n");
            sendMsg(userToKick.getFd(), ":ircserv 400 * :KICK " + channelName + " " + userName + " :" + comment + "\r\n");
            it->removeClient(userToKick);
            it->removeInvited(userToKick);
            it->removeOp(userToKick);
            return;
        }
    }
    sendMsg(client->getFd(), ":ircserv 403 * :No such channel\r\n");
    return;
}
