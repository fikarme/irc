#include "../Server.hpp"

void Server::help(Client* client, const vector<string>& params) {
	if (!client->getIsConnected()) {
		sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
		sendInfo(client);
		return;
	}
    if (!params.empty()) {
        sendMsg(client->getFd(), "Usage: HELP\r\n");
        return;
    }

    sendMsg(client->getFd(), ":ircserv 704 " + client->getNickn() + " :Available commands:\r\n");
	vector<string> helpLines;

	helpLines.push_back("USER <username> <hostname> <servername> <realname> - Register your user info");
	helpLines.push_back("PASS <password> - Authenticate with the server");
	helpLines.push_back("NICK <nickname> - Set your nickname");
	helpLines.push_back("JOIN <#channel> [password] - Join or create a channel");
	helpLines.push_back("KICK <#channel> <user> - Remove a user from a channel");
	helpLines.push_back("MODE <#channel> <+|- flags> - Modify channel modes (i,t,k,o,l)");
	helpLines.push_back("PART <#channel> [message] - Leave a channel");
	helpLines.push_back("TOPIC <#channel> [topic] - View or set the channel topic");
	helpLines.push_back("INVITE <user> <#channel> - Invite a user to a channel");
	helpLines.push_back("PRIVMSG <target> <message> - Send a private message");
	helpLines.push_back("QUIT [<reason>] - Disconnect from the server");
	helpLines.push_back("HELP - Display this help message");
	
    for (size_t i = 0; i < helpLines.size(); ++i)
        sendMsg(client->getFd(), ":ircserv 705 " + client->getNickn() + " :" + helpLines[i] + "\r\n");

    sendMsg(client->getFd(), ":ircserv 706 " + client->getNickn() + " :End of HELP\r\n");
}
