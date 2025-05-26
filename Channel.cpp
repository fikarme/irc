#include "Channel.hpp"

Channel::~Channel() {}


Channel::Channel(const string name, string key){
	_name = name;
	_key = key;
	_topic = "";
	_limit = 0;
	_hasLimit = false;
	_hasKey = false;
	_hasTopicProtection = false;
	_hasInviteOnly = false;
}

Channel::Channel(const Channel &rhs){
	*this = rhs;
}

Channel &Channel::operator=(const Channel &rhs) {
	if (this != &rhs) {
		_name = rhs._name;
		_topic = rhs._topic;
		_clients = rhs._clients;
		_invited = rhs._invited;
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

string Channel::getName() const {
	return _name;
}

string Channel::getTopic() const {
	return _topic;
}

void Channel::setName(const string name) {
	_name = name;
}

void Channel::setTopic(const string topic) {
	_topic = topic;
}

void Channel::addClient(Client client) {
	_clients.push_back(client);
}

void Channel::removeClient(Client client) {
    if (isIn(_clients, client)) {
		removeFromVector(_clients, client);
		removeOp(client);
		removeInvited(client);
	}
}

vector<Client>& Channel::getClients() {
	return _clients;
}

void Channel::addOp(Client client) {
	_ops.push_back(client);
}

void Channel::removeOp(Client client) {
    if (isIn(_ops, client))
        removeFromVector(_ops, client);
}

bool Channel::isOp(Client client) const {
    return(isIn(_ops, client));
}

vector<Client> Channel::getOps() const {
	return _ops;
}

void Channel::setKey(string key) {
	_key = key;
}

string Channel::getKey() const {
	return _key;
}

void Channel::setLimit(size_t limit) {
	_limit = limit;
}

size_t Channel::getLimit() const {
	return _limit;
}

void Channel::setHasLimit(bool hasLimit) {
	_hasLimit = hasLimit;
}

bool Channel::getHasLimit() const {
	return _hasLimit;
}

void Channel::setHasKey(bool hasKey) {
	_hasKey = hasKey;
}

bool Channel::getHasKey() const {
	return _hasKey;
}

void Channel::setHasTopicProtection(bool hasTopicProtection) {
	_hasTopicProtection = hasTopicProtection;
}

bool Channel::getHasTopicProtection() const {
	return _hasTopicProtection;
}

void Channel::setHasInviteOnly(bool hasInviteOnly) {
	_hasInviteOnly = hasInviteOnly;
}

bool Channel::getHasInviteOnly() const {
	return _hasInviteOnly;
}

void Channel::addInvited(Client client) {
	_invited.push_back(client);
}

void Channel::removeInvited(Client client) {
    if (isIn(_invited, client))
        removeFromVector(_invited, client);
}

bool Channel::isInvited(Client client) const {
    return(isIn(_invited, client));
}

void Channel::broadcastMsg(const string &msg) {
	for (vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		sendMsg(it->getFd(), msg);
}

bool Channel::isIn(const vector<Client> &clientList, const Client &client) const {
    for (size_t i = 0; i < clientList.size(); ++i)
        if (clientList[i].getNickn() == client.getNickn())
            return true;
    return false;
}

void Channel::updateNickn(const string &oldNickn, const string &newNickn) {
    for (size_t i = 0; i < _clients.size(); ++i)
        if (_clients[i].getNickn() == oldNickn)
            _clients[i].setNickn(newNickn);
    for (size_t i = 0; i < _ops.size(); ++i)
        if (_ops[i].getNickn() == oldNickn)
            _ops[i].setNickn(newNickn);
    for (size_t i = 0; i < _invited.size(); ++i)
        if (_invited[i].getNickn() == oldNickn)
            _invited[i].setNickn(newNickn);
}

void Channel::removeFromVector(vector<Client>& vec, const Client& client) {
    for (vector<Client>::iterator it = vec.begin(); it != vec.end(); ++it) {
        if (it->getNickn() == client.getNickn()) {
            vec.erase(it);
            return;
        }
    }
}

bool Channel::isInChannel(const string nick) const {
	for (size_t i = 0; i < _clients.size(); ++i)
		if (_clients[i].getNickn() == nick)
			return true;
	return false;
}

Client Channel::getClientByNick(const string nick) const {
	for (size_t i = 0; i < _clients.size(); ++i)
		if (_clients[i].getNickn() == nick)
			return _clients[i];
	return Client(-1);
}

string Channel::getMode() const {
	string mode = "";
	if (_hasKey)
		mode += "k";
	if (_hasLimit)
		mode += "l";
	if (_hasTopicProtection)
		mode += "t";
	if (_hasInviteOnly)
		mode += "i";
	return mode;
}

using std::cout;

void Channel::printChannel() const{
	cout << "Channel Name: " << this->_name << endl;
	cout << "Channel Topic: " << this->_topic << endl;
	cout << "Channel Has Key: " << (this->getHasKey() ? "Yes" : "No") << endl;
	cout << "Channel Key: " << this->_key << endl;
	cout << "Channel Has Limit: " << (this->getHasLimit() ? "Yes" : "No") << endl;
	cout << "Channel Limit: " << this->_limit << endl;
	cout << "Channel Has Topic Protection: " << (this->getHasTopicProtection()) << endl;
	cout << "Channel Has Invite Only: " << (this->getHasInviteOnly() ? "Yes" : "No") << endl;
	cout << "Channel Clients Count: " << this->_clients.size() << endl;
	cout << "Channel Ops Count: " << this->_ops.size() << endl;
	cout << "Channel Invited Count: " << this->_invited.size() << endl;

	cout << "Channel Invited: " << endl;
	for (vector<Client>::const_iterator it = this->_invited.begin(); it != this->_invited.end(); ++it)
		cout << "-" << it->getNickn() << endl;
	cout << "Channel Clients: " << endl;
	for (vector<Client>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		cout << "-" << it->getNickn() << endl;
	cout << "Channel Ops: " << endl;
	for (vector<Client>::const_iterator it = this->_ops.begin(); it != this->_ops.end(); ++it)
		cout << "-" << it->getNickn() << endl;
	cout << endl;
}
