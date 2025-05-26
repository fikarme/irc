#include "../Server.hpp"

void Server::invite(Server *server, Client *client, const vector<string> &params) {
    if (!client->getIsConnected()) {
		sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
		sendInfo(client);
		return;
	}
    if (params.size() == 0) {
        string channelNames;
        for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
            if (it->isInvited(*client)) {
                if (channelNames.empty())
                    channelNames = it->getName();
                else
                    channelNames += " " + it->getName();
            }
        }
        if (channelNames.empty()) {
            sendMsg(client->getFd(), ":ircserv 336 * :No channels\r\n");
            return;
        }
        sendMsg(client->getFd(), ":ircserv 336 " + clientPrefix(client) + " INVITELIST * :" + channelNames + "\r\n");
        return;
    }
    if (params.size() == 1) {
        sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
        sendMsg(client->getFd(), "Usage: `INVITE <nickname> <channel>`\r\n");
        return;
    }
    if (params.size() != 2) {
        sendMsg(client->getFd(), ":ircserv 461 * :Too many parameters\r\n");
        sendMsg(client->getFd(), "Usage: `INVITE <nickname> <channel>`\r\n");
        return;
    }
    
    string nick = params[0];
    string channel = params[1];

    if (channel.empty() || channel[0] != '#' || channel.find_first_of(" ,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 476 * :Invalid channel name\r\n");
        return;
    }
    if (nick.empty() || nick.find_first_of(" ,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 401 * :No such nick\r\n");
        return;
    }

    bool nickFound = false;
    Client* invitedClient = NULL;
	const std::map<int, Client*>& clients = server->getOurClients();
    for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second && it->second->getNickn() == nick) {
            nickFound = true;
            invitedClient = it->second;
            break;
        }
    }
    if (!nickFound) {
        sendMsg(client->getFd(), ":ircserv 401 * :No such nick\r\n");
        return;
    }

    for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
        if (it->getName() == channel) {
            if (!it->isInChannel(client->getNickn())) {
                sendMsg(client->getFd(), ":ircserv 442 * :You're not on that channel\r\n");
                return;
            }
            if (!it->isOp(*client) && it->getHasInviteOnly()) {
                sendMsg(client->getFd(), ":ircserv 482 * :You're not channel operator\r\n");
                return;
            }
            if (it->isInChannel(nick)) {
                sendMsg(client->getFd(), ":ircserv 443 * :User is already on channel\r\n");
                return;
            }
            if (it->isInvited(*invitedClient)) {
                sendMsg(client->getFd(), ":ircserv 341 * :" + clientPrefix(client) + " INVITE " + nick + " " + channel + "\r\n");
                return;
            }

            it->addInvited(*invitedClient); 
            sendMsg(client->getFd(), ":ircserv 341 * :" + clientPrefix(client) + " INVITE " + nick + " " + channel + "\r\n");
            sendMsg(invitedClient->getFd(), client->getNickn() + " invited you to this channel -> " + channel + "\r\n");
            return;
        }
    }
    sendMsg(client->getFd(), ":ircserv 403 * :No such channel\r\n");
    return;
}
