#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "Channel.hpp"
#include "Client.hpp"
#include "Utils.hpp"

#define CLIENT_LIMIT 100
#define BUFFER_SIZE 512

using std::map;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

class Server {
	private:
		string					password;
		unsigned int			port;
		int						socket_fd;
		int						polling;
		vector<struct pollfd>	polling_fds;
		map<int, Client*>		ourClients;
		vector<Channel>			channels;

        map<string, void (Server::*)(Client*, const vector<string>&)> commander;
        map<string, void (Server::*)(Server*, Client*, const vector<string>&)> serverCommander;

		void setupServer(void);
		void setupCommands(void);
		void acceptClient(void);
		void handleMessage(int fd);
		void removeClient(Client *client);
		void processLine(Client *client, const string &line);
		bool nonBlockingSocket(int socket_fd);


	public:
		Server(const string& port, const string& password);
		~Server();
		static bool			isValidPort(const string &s);
		string				getServerPass() const;
		vector<Channel>&	getChannels();
		map<int, Client*>&	getOurClients();
		const string		clientPrefix(Client *client);
		
		void loop();
		void sendInfo(Client *client);
		void updateServerWideNickN(const string &newNickN, int fd);
		void createChannel(Client *client, string channelName, string key);
		void deleteChannel(string channelName);
		
		void printServer();

		void who(Client *client, const vector<string> &params);
		void cap(Client *client, const vector<string> &params);
		void help(Client *client, const vector<string> &params);
		void user(Client *client, const vector<string> &params);
		void ping(Client *client, const vector<string> &params);
		void notice(Client *client, const vector<string> &params);

		void quit(Server *server, Client *client, const vector<string> &params);
		void part(Server *server, Client *client, const vector<string> &params);
		void pass(Server *server, Client *client, const vector<string> &params);
		void nick(Server *server, Client *client, const vector<string> &params);
		void join(Server *server, Client *client, const vector<string> &params);
		void kick(Server *server, Client *client, const vector<string> &params);
		void mode(Server *server, Client *client, const vector<string> &params);
		void topic(Server *server, Client *client, const vector<string> &params);
		void invite(Server *server, Client *client, const vector<string> &params);
		void privmsg(Server *server, Client *client, const vector<string> &params);
};

#endif
