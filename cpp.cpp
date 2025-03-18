#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Server.hpp"

using std::string;
using std::vector;
class Client{
private:
        int     _fd;
        string  _nickn;
        string  _usern;
        string  _hostn;
        string  _servern;
        string  _realn;
        string  _ip;
        string  _pass;
        bool    _isAuth;
        bool    _isOp;


public:
	~Client();
        Client(int fd);
        Client(const Client &rhs);
        Client &operator=(const Client &rhs); //right hand side

        const string &getNickn() const;
        const string &getUsern() const;
        const string &getHostn() const;
        const string &getServern() const;
        const string &getRealn() const;
        bool getIsAuth() const;
        bool getIsOp() const;
        int getFd() const;

        void setNickn(const string nickn);
        void setUsern(const string usern);
        void setHostn(const string hostn);
        void setServern(const string servern);
        void setRealn(const string realn);
        void setIsAuth(bool isAuth);
        void setIsOp(bool isOp);
        void setFd(int fd);

};

#endif

#include "Channel.hpp"

Channel::~Channel(){}

Channel::Channel(const string name, string topic) : _name(name), _topic(topic){}

Channel::Channel(const Channel &rhs) : _name(rhs._name), _topic(rhs._topic), _clients(rhs._clients), _ops(rhs._ops){}

Channel &Channel::operator=(const Channel &rhs){
	if(this != &rhs){
		_name = rhs._name;
		_topic = rhs._topic;
		_clients = rhs._clients;
		_ops = rhs._ops;
		_key = rhs._key;
		_limit = rhs._limit;
		_hasLimit = rhs._hasLimit;
		_hasKey = rhs._hasKey;
		_hasTopicProtection = rhs._hasTopicProtection;
		_hasInviteOnly = rhs._hasInviteOnly;
	}
	return *this;
}

string Channel::getName() const{
	return _name;
}

string Channel::getTopic() const{
	return _topic;
}

void Channel::setName(const string name){
	_name = name;
}

void Channel::setTopic(const string topic){
	_topic = topic;
}

void Channel::addClient(Client client){
	_clients.push_back(client);
}

void Channel::removeClient(Client client){
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it){
        if (it->getNickn() == client.getNickn()){
            _clients.erase(it);
            break;
        }
    }
}

vector<Client> Channel::getClients() const{
	return _clients;
}

void Channel::addOp(Client client){
	_ops.push_back(client);
}

void Channel::removeOp(Client client){
    for (std::vector<Client>::iterator it = _ops.begin(); it != _ops.end(); ++it){
        if (it->getNickn() == client.getNickn()){
            _ops.erase(it);
            break;
        }
    }
}

bool Channel::isOp(Client client) const{
    for (std::vector<Client>::const_iterator it = _ops.begin(); it != _ops.end(); ++it){
        if (it->getNickn() == client.getNickn()){
            return true;
        }
    }
    return false;
}

vector<Client> Channel::getOps() const{
	return _ops;
}

void Channel::setKey(string key){
	_key = key;
}

string Channel::getKey() const{
	return _key;
}

void Channel::setLimit(int limit){
	_limit = limit;
}

int Channel::getLimit() const{
	return _limit;
}

void Channel::setHasLimit(bool hasLimit){
	_hasLimit = hasLimit;
}

bool Channel::getHasLimit() const{
	return _hasLimit;
}

void Channel::setHasKey(bool hasKey){
	_hasKey = hasKey;
}

bool Channel::getHasKey() const{
	return _hasKey;
}

void Channel::setHasTopicProtection(bool hasTopicProtection){
	_hasTopicProtection = hasTopicProtection;
}

bool Channel::getHasTopicProtection() const{
	return _hasTopicProtection;
}

void Channel::setHasInviteOnly(bool hasInviteOnly){
	_hasInviteOnly = hasInviteOnly;
}

bool Channel::getHasInviteOnly() const{
	return _hasInviteOnly;
}

bool Channel::isClientInChannel(string nickn) const{
	for (vector<Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it){
		if (it->getNickn() == nickn){
			return true;
		}
	}
	return false;
}

void Channel::addInvited(Client client){
	_invited.push_back(client);
}

void Channel::removeInvited(Client client){
	for (vector<Client>::iterator it = _invited.begin(); it != _invited.end(); ++it){
		if (it->getNickn() == client.getNickn()){
			_invited.erase(it);
			break;
		}
	}
}

bool Channel::isInvited(Client client) const{
	for (vector<Client>::const_iterator it = _invited.begin(); it != _invited.end(); ++it){
		if (it->getNickn() == client.getNickn()){
			return true;
		}
	}
	return false;
}

#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Server.hpp"

class Channel{
	private:
		string _name;
		string _topic;
		vector<Client> _clients;
		vector<Client> _ops;
		vector<Client> _invited;

		string _key;
		int _limit;
		bool _hasLimit;
		bool _hasKey;
		bool _hasTopicProtection;
		bool _hasInviteOnly;


	public:
		~Channel();
		Channel(const Channel &rhs);
		Channel(const string name, string topic);
		Channel &operator=(const Channel &rhs); //right hand side

		string getName() const;
		string getTopic() const;
		void setName(const string name);
		void setTopic(const string topic);

		void addClient(Client client);
		void removeClient(Client client);
		vector<Client> getClients() const;

		void addOp(Client client);
		void removeOp(Client client);
		bool isOp(Client client) const;
		vector<Client> getOps() const;

		void setKey(string key);
		string getKey() const;

		void setLimit(int limit);
		int getLimit() const;

		void setHasLimit(bool hasLimit);
		bool getHasLimit() const;

		void setHasKey(bool hasKey);
		bool getHasKey() const;

		void setHasTopicProtection(bool hasTopicProtection);
		bool getHasTopicProtection() const;

		void setHasInviteOnly(bool hasInviteOnly);
		bool getHasInviteOnly() const;

		bool isClientInChannel(string nickn) const;

		void addInvited(Client client);;
		void removeInvited(Client client);
		bool isInvited(Client client) const;


};

#endif
