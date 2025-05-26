#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <cstdio>
#include <string>
#include <unistd.h>

#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define RESET	"\033[0m"

using std::string;
using std::cerr;
using std::endl;

void    coloredError(const string &color, const string &message);
void    sendMsg(int fd, const string &msg);
string  intToString(unsigned int port);
size_t  stringToSizeT(const string &str);

#endif
