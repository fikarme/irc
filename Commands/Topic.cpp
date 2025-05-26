#include "../Server.hpp"

void Server::topic(Server *server, Client *client, const vector<string> &params) {
     if (!client->getIsConnected()) {
		sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
		sendInfo(client);
		return;
	}
    if (params.size() < 1) {
        sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
        sendMsg(client->getFd(), "Usage: `TOPIC <channel> [topic]`\r\n");
        return;
    }

    string channelName = params[0];
    string topic = "";
    bool checkTopic = false;
    bool clearTopic = false;

    if (params.size() == 1)
        checkTopic = true;
    if (params.size() == 2 && params[1] == ":")
        clearTopic = true;
    else if (params.size() > 1) {
        topic = params[1];
        if (topic[0] == ':') {
            topic.erase(0, 1);
            for (size_t i = 2; i < params.size(); i++)
                topic += " " + params[i];
        }
        else if (params.size() > 2) {
            sendMsg(client->getFd(), ":ircserv 461 * :Invalid topic add ':' at the beginning\r\n");
            return;
        }
    }
    if (topic.find_first_of(":,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 400 * :Invalid topic\r\n");
        return;
    }
    if (channelName.empty() || channelName[0] != '#' || channelName.find_first_of(" ,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 476 * :Invalid channel name\r\n");
        return;
    }

    for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
        if (it->getName() == channelName) {
            if (!it->isInChannel(client->getNickn())) {
                sendMsg(client->getFd(), ":ircserv 442 * :You're not on that channel\r\n");
                return;
            }
            if (checkTopic) {
                if (it->getTopic().empty())
                sendMsg(client->getFd(), ":ircserv 331 * :No topic is set!\r\n");
                else
                    sendMsg(client->getFd(), ":ircserv 332 " + clientPrefix(client) + " TOPIC " + channelName + " :" + it->getTopic() + "\r\n");
                return;
            }
            if (!it->isOp(*client) && it->getHasTopicProtection()) {
                sendMsg(client->getFd(), ":ircserv 482 * :You're not channel operator\r\n");
                return;
            }
            if (topic.find_first_of(",\a") != string::npos) {
                sendMsg(client->getFd(), ":ircserv 461 * :Invalid topic\r\n");
                return;
            }
            if (clearTopic)
                topic = "No topic is set";
            it->setTopic(topic);
            it->broadcastMsg(clientPrefix(client) + " TOPIC " + channelName + " :" + topic + "\r\n");
            sendMsg(client->getFd(), ":ircserv 332 " + clientPrefix(client) + " TOPIC " + channelName + " :" + topic + "\r\n");
            return;
        }
    }
    sendMsg(client->getFd(), ":ircserv 403 * :No such channel\r\n");
    return;
}
