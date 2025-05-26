#include "../Server.hpp"

void Server::notice(Client *client, const vector<string> &params) {
    if (!client->getIsAuth()) {
        sendMsg(client->getFd(), ":ircserv 451 * :You should authorize first\r\n");
        return;
    }
    if (params.size() > 1) {
        const string& param = params[1];
        if (param.length() >= 10 && param[0] == ':') {
            if (param[2] == 'L' && param[3] == 'A' && param[4] == 'G' &&
                param[5] == 'C' && param[6] == 'H' && param[7] == 'E' &&
                param[8] == 'C' && param[9] == 'K' && param[1] == 1)
                return;
        }
    }
    sendMsg(client->getFd(), ":ircserv 421 * :Unknown command\r\n");
    return;
}
