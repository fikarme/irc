#include "../Server.hpp"

void Server::join(Server *server, Client *client, const vector<string> &params) {
    if (!client->getIsConnected()) {
		sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
		sendInfo(client);
		return;
	}
    if (params.size() < 1) {
        sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
        sendMsg(client->getFd(), "Usage: `JOIN <channel> [key]`\r\n");
        return;
    }
    if (params.size() > 2) {
        sendMsg(client->getFd(), ":ircserv 461 * :Too many parameters\r\n");
        sendMsg(client->getFd(), "Usage: `JOIN <channel> [key]`\r\n");
        return;
    }

    string channelName = params[0];

    if (channelName == "0") {
        vector<string> channelsToDelete;
        for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
            if (it->isInChannel(client->getNickn())) {
                if (it->isOp(*client) && it->getClients().size() > 1 && it->getOps().size() == 1) {
                    sendMsg(client->getFd(), ":ircserv 400 * :Make someone else operator at" + it->getName() + "\r\n");
                    return;
                }
                it->removeClient(*client);
                it->removeInvited(*client);
                it->removeOp(*client);
                sendMsg(client->getFd(), ":ircserv 400 * :PART " + it->getName() + " " + client->getNickn() + "\r\n");
                it->broadcastMsg(clientPrefix(client) + " PART :" + it->getName() + "\r\n");
                if (it->getClients().size() == 0)
                    channelsToDelete.push_back(it->getName());
            }
        }
        vector<string>::iterator nameIt = channelsToDelete.begin();
             for (; nameIt != channelsToDelete.end(); ++nameIt) {
                   server->deleteChannel(*nameIt);
            }
        return;
    }

	if (channelName.empty() || channelName[0] != '#' || channelName.find_first_of(" ,\a") != string::npos){
        sendMsg(client->getFd(), ":ircserv 476 * :Invalid channel name\r\n");
        return;
    }

	string key = "";
    
	if (params.size() == 2) {
		key = params[1];
		if (key.find_first_of(" :#\r\n") != string::npos) {
			sendMsg(client->getFd(), ":ircserv 475 * :Invalid key\r\n");
			return;
		}
	}

    for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it){
        if (it->getName() == channelName) {
            if (it->isInChannel(client->getNickn())) {
                sendMsg(client->getFd(), ":ircserv 405 * :Already in channel\r\n");
                return;
            }
            if (it->getHasLimit() && (it->getLimit() <= it->getClients().size())) {
                sendMsg(client->getFd(), ":ircserv 471 * :Channel is full\r\n");
                return;
            }
            if (it->getHasInviteOnly() && !it->isInvited(*client)) {
                sendMsg(client->getFd(), ":ircserv 473 * :You are not invited\r\n");
                return;
            }
            if (it->getHasKey() && it->getKey() != key) {
                sendMsg(client->getFd(), ":ircserv 475 * :Invalid key\r\n");
                return;
            }
            it->addClient(*client);
            string joinMessage = ":" + client->getNickn() + "!" + client->getUsern() + "@" + client->getIp() + " JOIN " + channelName + "\r\n";
	        it->broadcastMsg(joinMessage);
            if (it->getTopic().empty())
                sendMsg(client->getFd(), ":ircserv 331 * :No topic is set!\r\n");
            else
                sendMsg(client->getFd(), ":ircserv 332 * :" + it->getTopic() + "\r\n");
			it->removeInvited(*client);
            string userList;
			for (vector<Client>::iterator it2 = it->getClients().begin(); it2 != it->getClients().end(); ++it2) {
    			if (it->isOp(*it2))
        			userList += "@" + it2->getNickn() + " ";
    			else
        			userList += it2->getNickn() + " ";
			}
			if (!userList.empty() && userList[userList.size() - 1] == ' ')
                userList.erase(userList.size() - 1);

	        sendMsg(client->getFd(), ":ircserv 353 "  + client->getNickn() + " = " + channelName + " :" + userList + "\r\n");
	        sendMsg(client->getFd(), ":ircserv 366 " + client->getNickn() + " " + channelName + " :End of /NAMES list.\r\n");

            return;
        }
    }
    server->createChannel(client, channelName, key);
    return;
}
