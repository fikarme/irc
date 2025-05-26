#include "Client.hpp"

Client::~Client() {}

Client::Client(int fd) {
    _fd = fd;
    _nickn = "";
    _usern = "";
    _hostn = "";
    _servern = "";
    _realn = "";
	_ip = "";
    _isAuth = false;
    _isOp = false;
    _isConnected = false;
    read_buffer = "";
}

Client::Client(const Client &rhs) {
    *this = rhs;
}

Client &Client::operator=(const Client &rhs) {
    if (this != &rhs) {
        _fd = rhs._fd;
        _nickn = rhs._nickn;
        _usern = rhs._usern;
        _hostn = rhs._hostn;
        _servern = rhs._servern;
		_ip = rhs._ip;
        _realn = rhs._realn;
        _isAuth = rhs._isAuth;
        _isOp = rhs._isOp;
        _isConnected = rhs._isConnected;
    }
    return *this;
}

int Client::getFd() const {
    return _fd;
}

const string &Client::getNickn() const {
    return _nickn;
}

const string &Client::getUsern() const {
    return _usern;
}

const string &Client::getHostn() const {
    return _hostn;
}

const string &Client::getServern() const {
    return _servern;
}

const string &Client::getRealn() const {
    return _realn;
}

const string &Client::getIp() const {
	return _ip;
}

bool Client::getIsAuth() const {
    return _isAuth;
}

bool Client::getIsOp() const {
    return _isOp;
}

bool Client::getIsConnected() const {
    return _isConnected;
}

void Client::setFd(int fd) {
    _fd = fd;
}

void Client::setIp(const string ip)
{
	_ip = ip;
}

void Client::setNickn(const string nickn) {
    _nickn = nickn;
}

void Client::setUsern(const string usern) {
    _usern = usern;
}

void Client::setHostn(const string hostn) {
    _hostn = hostn;
}

void Client::setServern(const string servern) {
    _servern = servern;
}

void Client::setRealn(const string realn) {
    _realn = realn;
}

void Client::setIsAuth(bool isAuth) {
    _isAuth = isAuth;
}

void Client::setIsOp(bool isOp) {
    _isOp = isOp;
}

void Client::setIsConnected(bool isConnected) {
    _isConnected = isConnected;
}
