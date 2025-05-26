#include "Utils.hpp"

void coloredError(const string &color, const string &message) {
	cerr << color << message << RESET << endl;
}

string intToString(unsigned int port) {
    char buffer[20];
	snprintf(buffer, sizeof(buffer), "%u", port);
    return string(buffer);
}

void sendMsg(int fd, const string &msg) {
	ssize_t result = send(fd, msg.c_str(), msg.size(), 0);

    if (result == -1)
		coloredError(RED, "ERROR: Send function failed!");
}

size_t stringToSizeT(const string &str) {
    size_t result = 0;
    for (size_t i = 0; i < str.length(); ++i)
        if (isdigit(str[i]))
            result = result * 10 + (str[i] - '0');
    return result;
}
