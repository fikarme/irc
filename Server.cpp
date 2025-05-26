#include "Server.hpp"
#include "Channel.hpp"

Server::Server(const string &port, const string &password) {
	this->port = atoi(port.c_str());
	this->password = password;
	this->socket_fd = 0;
	this->polling = 0;
	setupServer();
	setupCommands();
}

Server::~Server() {
	if(socket_fd >= 0)
		close(socket_fd);
	for (map<int, Client*>::iterator it = ourClients.begin(); it != ourClients.end(); ++it) {
		close(it->second->getFd());
		delete it->second;
	}

	polling_fds.clear();
	ourClients.clear();
	channels.clear();

	coloredError(GREEN, "Server is shutting down...");
}

void Server::setupCommands() {
	commander["WHO"] = &Server::who;
    commander["CAP"] = &Server::cap;
    commander["HELP"] = &Server::help;
    commander["USER"] = &Server::user;
    commander["PING"] = &Server::ping;
    commander["NOTICE"] = &Server::notice;
    serverCommander["PASS"] = &Server::pass;
    serverCommander["NICK"] = &Server::nick;
    serverCommander["JOIN"] = &Server::join;
    serverCommander["KICK"] = &Server::kick;
    serverCommander["MODE"] = &Server::mode;
    serverCommander["TOPIC"] = &Server::topic;
    serverCommander["INVITE"] = &Server::invite;
    serverCommander["PRIVMSG"] = &Server::privmsg;
	serverCommander["QUIT"] = &Server::quit;
	serverCommander["PART"] = &Server::part;
}

void Server::setupServer() {
	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0)
		throw std::runtime_error("Socket failed");
	
	if (!nonBlockingSocket(socket_fd)) {
		close(socket_fd);
		throw std::runtime_error("Socket failed");
	}

	int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close (socket_fd);
		throw std::runtime_error("Set socket options failed");
	}

	struct sockaddr_in address;
	std::memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		close (socket_fd);
		throw std::runtime_error("Bind function failed");
    }
	else
		coloredError(GREEN, "Server is up and running on port " + intToString(port) + "!");

	if (listen(socket_fd, SOMAXCONN) < 0) {
		close (socket_fd);
		throw std::runtime_error("Listen function failed");
	}
	else
		coloredError(GREEN, "Listening for incoming connections...");

	struct pollfd poll_fd;
    poll_fd.fd = socket_fd;
    poll_fd.events = POLLIN;
    polling_fds.push_back(poll_fd);
}

void Server::acceptClient() {
    sockaddr_in client_address;
    socklen_t cladr_len = sizeof(client_address);
    int client_fd = accept(socket_fd, (struct sockaddr *)&client_address, &cladr_len);

    if (client_fd == -1) {
        coloredError(RED, "ERROR: Accept function failed!");
        return;
    }

    if (ourClients.size() >= CLIENT_LIMIT) {
        string message = "ERROR: Server is full of clients right now, we can't accept you ｡°(°.◜ᯅ◝°)°｡ !";
		sendMsg(client_fd, message);
        close(client_fd);
        return;
    }

	nonBlockingSocket(client_fd);

	int opt = 1;
	if(setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) == -1)
	{
		close(client_fd);
		coloredError(RED, "ERROR: Set socket options failed!");
		return;
	}
	
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_address.sin_addr), ip, INET_ADDRSTRLEN);
	string client_ip(ip);

	ourClients[client_fd] = new Client(client_fd);
	ourClients[client_fd]->setIp(client_ip);
	
    struct pollfd client_pollfd;
    client_pollfd.fd = client_fd;
    client_pollfd.events = POLLIN;
    polling_fds.push_back(client_pollfd);

	coloredError(GREEN, "New client connected: FD " + intToString(client_fd) + "!");
	sendMsg(client_fd, ":" + ourClients[client_fd]->getIp() + " NOTICE * :I don't know who you are ( •̀ - • ), you must log in with password first!\r\n");
}

void Server::removeClient(Client *client) {
    int fd = client->getFd();

    close(fd);

    std::map<int, Client*>::iterator it = ourClients.find(fd);
    if (it != ourClients.end()) {
        delete it->second;
        ourClients.erase(it);
    }

    for (vector<struct pollfd>::iterator it = polling_fds.begin(); it != polling_fds.end(); ++it)
    {
        if (it->fd == fd)
        {
            polling_fds.erase(it);
            break;
        }
    }

	coloredError(RED, "Client disconnected: FD " + intToString(fd) + "!");
}

void Server::processLine(Client *client, const string &line) {
	std::istringstream iss(line);
    string command;
    iss >> command;

    vector <string> args;
    string temp;
    while (iss >> temp) {
        args.push_back(temp);
	}
	printf("Command: %s\n", command.c_str());
	for (size_t i = 0; i < args.size(); ++i)
		printf("Arg %zu: %s\n", i, args[i].c_str());

    if (commander.find(command) != commander.end())
        (this->*(this->commander[command]))(client, args);
    else if (serverCommander.find(command) != serverCommander.end())
        (this->*(this->serverCommander[command]))(this, client, args);
	else
		sendMsg(client->getFd(), ":ircserv 421 * :Unknown command\r\n");
}

void Server::handleMessage(int fd) {
    char buffer[BUFFER_SIZE];

    int receivedBytes = recv(fd, buffer, BUFFER_SIZE - 1, 0);

	if (receivedBytes == 0)
	{
        Client *client = ourClients[fd];
        removeClient(client);
        return;
    }
	if (receivedBytes < 0)
	{
		coloredError(RED, "ERROR: Receive function failed!");
		return;
	}

    buffer[receivedBytes] = '\0';
    string message(buffer);
	Client *client = ourClients[fd];

	client->read_buffer += buffer;

	size_t pos;
    while ((pos = client->read_buffer.find("\n")) != string::npos)
	{
        string command = client->read_buffer.substr(0, pos);
        client->read_buffer.erase(0, pos + 1);
        if (!command.empty())
            processLine(client, command);
    }
}

void Server::loop() {
	while(true) {
		polling = poll(polling_fds.data(), polling_fds.size(), -1);
		if (polling < 0)
		{
			throw std::runtime_error("Poll function failed");
		}
		for (size_t i = 0; i < polling_fds.size(); ++i)
        {
            if (polling_fds[i].revents & POLLIN)
            {
                if (polling_fds[i].fd == socket_fd)
                    acceptClient();
                else
                    handleMessage(polling_fds[i].fd);
            }
        }
	}
}

bool Server::nonBlockingSocket(int socket_fd) {
    if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) == -1)
		return false;
	return true;
}

bool Server::isValidPort(const string &s) {
	unsigned int i = 0;
	while (i < s.size() && std::isdigit(s[i]))
		i++;
	if(i != s.size())
		return false;
	int port = std::atoi(s.c_str());
	if ((port > 0 && port <= 65535) && i <= 5)
		return true;
	return false;
}

string Server::getServerPass() const {
	return password;
}

void Server::updateServerWideNickN(const string &newNickN, int fd) {
	for (std::map<int, Client*>::iterator it = ourClients.begin(); it != ourClients.end(); ++it)
		if (it->second->getFd() == fd)
			it->second->setNickn(newNickN);
}

void Server::createChannel(Client *client, string channelName, string key) {
    Channel newChan(channelName, key);
    newChan.addClient(*client);
    newChan.addOp(*client);
	if (key != "")
	{
		newChan.setKey(key);
		newChan.setHasKey(true);
	}
    channels.push_back(newChan);

	string nick = client->getNickn();

	string joinMessage = ":" + nick + "!" + client->getUsern() + "@" + client->getIp() + " JOIN " + channelName + "\r\n";
	sendMsg(client->getFd(), joinMessage);
	sendMsg(client->getFd(), ":ircserv 331 :No topic is set!\r\n");
	sendMsg(client->getFd(), ":ircserv 353 "  + channelName + " :@" + client->getNickn() + "\r\n");
	sendMsg(client->getFd(), ":ircserv 366 " + channelName + " :End of /NAMES list.\r\n");
	sendMsg(client->getFd(), ":ircserv MODE " + channelName + " +o " + client->getNickn() + "\r\n");
}

void Server::deleteChannel(string channelName) {
	for (vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->getName() == channelName) {
			if (it->getClients().empty()) {
				channels.erase(it);
				break;
			}
		}
	}
}

const string Server::clientPrefix(Client *client) {
	return ":" + client->getNickn() + "!" + client->getUsern() + "@" + client->getIp();
}

void Server::sendInfo(Client *client) {	
	if (!client->getIsAuth())
		sendMsg(client->getFd(), ":ircserv 464 * :You should authorize first\r\n");
	else if (client->getIsAuth() && client->getNickn().empty())
		sendMsg(client->getFd(), ":ircserv 431 * :No nickname given\r\n");
	else if (client->getIsAuth() && !client->getNickn().empty() && client->getUsern().empty())
		sendMsg(client->getFd(), ":ircserv 461 * :No username given\r\n");
}

map<int, Client*>& Server::getOurClients() {
	return ourClients;
}

vector<Channel>& Server::getChannels() {
	return channels;
}

using std::cout;

void Server::printServer() {
    cout << "Server socket_fd: " << this->socket_fd << endl;
    cout << "Server Port: " << this->port << endl;
    cout << "Server Password: " << this->password << endl;
    cout << "Printing all clients: " << endl;
    for (map<int, Client*>::iterator it = ourClients.begin(); it != ourClients.end(); ++it) {
        cout << "Client sockfd: " << it->second->getFd() << endl;
        cout << "Client Nickname: " << it->second->getNickn() << endl;
        cout << "Client Username: " << it->second->getUsern() << endl;
        cout << "Client Hostname: " << it->second->getHostn() << endl;
        cout << "Client Servername: " << it->second->getServern() << endl;
        cout << "Client Realname: " << it->second->getRealn() << endl;
        cout << "Client Connected: " << (it->second->getIsConnected() ? "Yes" : "No") << endl;
		cout << "Client Auth: " << (it->second->getIsAuth() ? "Yes" : "No") << endl;
        cout << "-" << endl;
    }
    cout << "Printing all channels: " << endl;
	for (vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		cout << "Channel Name: " << it->getName() << endl;
		cout << "Channel Topic: " << it->getTopic() << endl;
		cout << "Channel Has Key: " << (it->getHasKey() ? "Yes" : "No") << endl;
		cout << "Channel Key: " << it->getKey() << endl;
		cout << "Channel Has Limit: " << (it->getHasLimit() ? "Yes" : "No") << endl;
		cout << "Channel Limit: " << it->getLimit() << endl;
		cout << "Channel Has Topic Protection: " << (it->getHasTopicProtection() ? "Yes" : "No") << endl;
		cout << "Channel Has Invite Only: " << (it->getHasInviteOnly() ? "Yes" : "No") << endl;
        cout << "-" << endl;
	}
    cout << endl;
}
