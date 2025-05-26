#include "../Server.hpp"

void Server::privmsg(Server *server, Client* client, const vector<string> &params){
	if (!client->getIsConnected()) {
		sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
		sendInfo(client);
		return;
	}
	if (params.size() < 2) {
		sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
		sendMsg(client->getFd(), "Usage: `PRIVMSG <channel> <message>`\r\n");
		return;
	}
	string receiver = params[0];
	string message = "";
	if (!params[1].empty() && params[1][0] == ':') {
		message = params[1];
		message.erase(0, 1);
		for(size_t i = 2; i < params.size(); i++)
			message += " " + params[i];
	}
	else {
		sendMsg(client->getFd(), ":ircserv 412 * :No text to send\r\n");
		return;
	}

	if (!receiver.empty() && receiver[0] != '#') {
    	const std::map<int, Client*>& clients = server->getOurClients();
		if (receiver[0] == ':')
			receiver.erase(0, 1);
    	for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
    		if (it->second && (it->second->getNickn() == receiver)) {
            	sendMsg(it->second->getFd(), ":" + clientPrefix(client) + " PRIVMSG " + receiver + " :" + message + "\r\n");
            	return;
            }
    	}
    	sendMsg(client->getFd(), ":ircserv 401 " + receiver + " :No such nick\r\n");
    	return;
	}

	if (receiver.empty() || (receiver[0] == '#' && receiver.find_first_of(" ,\a") != string::npos)) {
		sendMsg(client->getFd(), ":ircserv 476 * :Invalid channel name\r\n");
		return;
	}

	for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
		if (it->getName() == receiver){
			if (!it->isInChannel(client->getNickn())) {
				sendMsg(client->getFd(), ":ircserv 442 * :You're not on that channel\r\n");
				return;
			}

			for (vector<Client>::iterator it2 = it->getClients().begin(); it2 != it->getClients().end(); ++it2)
			{	
				if(it2->getNickn() != client->getNickn())
					sendMsg(it2->getFd(), ":" + clientPrefix(client) + " PRIVMSG " + receiver + " :" + message + "\r\n");
			}
			return;
		}
	}
	sendMsg(client->getFd(), ":ircserv 403 * :No such channel\r\n");
	return;
}
