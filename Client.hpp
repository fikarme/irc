#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

class Client {
    private:
        int     _fd;
        string  _nickn;
        string  _usern;
        string  _hostn;
        string  _servern;
        string  _realn;
        string  _ip;
        bool    _isAuth;
        bool    _isOp; 
        bool    _isConnected;
    public:
		string  read_buffer;

        ~Client();
        Client(int fd);
        Client(const Client &rhs);
        Client &operator=(const Client &rhs);
        const string &getNickn() const;
        const string &getUsern() const;
        const string &getHostn() const;
        const string &getServern() const;
        const string &getRealn() const;
		const string &getIp() const;
        bool getIsAuth() const;
        bool getIsOp() const;
        int getFd() const; 
        bool getIsConnected() const;
        void setNickn(const string nickn);
        void setUsern(const string usern);
        void setHostn(const string hostn);
        void setServern(const string servern);
        void setRealn(const string realn);
        void setIsAuth(bool isAuth);
        void setIsOp(bool isOp);
        void setFd(int fd);
		void setIp(const string ip);
        void setIsConnected(bool isConnected);
};

#endif
