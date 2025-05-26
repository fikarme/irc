#include "../Server.hpp"

void Server::mode(Server *server, Client *client, const vector<string> &params) {
    if (!client->getIsConnected()) {
        sendMsg(client->getFd(), ":ircserv 451 * :You have not registered\r\n");
		sendInfo(client);
        return;
    }
    if (params.size() < 1) {
        sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
        sendMsg(client->getFd(), "Usage: `MODE <target> [<modestring> [<mode arguments>...]]`\r\n");
        return;
    }
    if (params.size() > 4) {
        sendMsg(client->getFd(), ":ircserv 461 * :Too many parameters\r\n");
        sendMsg(client->getFd(), "Usage: `MODE <target> [<modestring> [<mode arguments>...]]`\r\n");
        return;
    }

    string target = params[0];
    string modeFlag = "";
    string modeStr = "";

    if (target[0] != '#' || !target[1] || target.find_first_of(" ,\a") != string::npos) {
        sendMsg(client->getFd(), ":ircserv 476 * :Invalid channel name\r\n");
        return;
    }
    if (params.size() != 1) {
        modeFlag = params[1];
        if (   modeFlag == "+k" || modeFlag == "-k"
            || modeFlag == "+o" || modeFlag == "-o"
            || modeFlag == "+l" || modeFlag == "-l") {
            if (params.size() != 3) {
                sendMsg(client->getFd(), ":ircserv 461 * :Need more parameters\r\n");
                sendMsg(client->getFd(), "Usage: `MODE <target> [<modestring> [<mode arguments>...]]`\r\n");
                return;
            }
            modeStr = params[2];
            if (modeStr.find_first_of(" ,\a") != string::npos) {
                sendMsg(client->getFd(), ":ircserv 461 * :Invalid mode string\r\n");
                return;
            }
        }
        else if (params.size() > 3) {
            sendMsg(client->getFd(), ":ircserv 461 * :Too many parameters\r\n");
            sendMsg(client->getFd(), "Usage: `MODE <target> [<modestring> [<mode arguments>...]]`\r\n");
            return;
        }
    }
    for (vector<Channel>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it) {
        if (it->getName() == target) {
            if (!it->isInChannel(client->getNickn())) {
                sendMsg(client->getFd(), ":ircserv 442 * :You're not on that channel\r\n");
                return;
            }
            if (params.size() == 1) {
                sendMsg(client->getFd(), ":ircserv 324 " + clientPrefix(client) + " MODE " + target + " :" + it->getMode() + "\r\n");
                return;
            }
            if (modeFlag == "b")
                return;
            if (!it->isOp(*client) ) {
                sendMsg(client->getFd(), ":ircserv 482 * :You're not channel operator\r\n");
                return;
            }
            if (modeFlag == "+o" || modeFlag == "-o") {
                for (vector<Client>::iterator opIt = it->getClients().begin(); opIt != it->getClients().end(); ++opIt) {
                    if (opIt->getNickn() == modeStr) {
                        if (modeFlag == "-o" && client->getNickn() == opIt->getNickn() && it->getOps().size() == 1) {
                            sendMsg(client->getFd(), ":ircserv 400 * :Make someone else operator\r\n");
                            return;
                        }
                        if (modeFlag == "+o") {
                            it->addOp(*opIt);
                            sendMsg(opIt->getFd(), ":ircserv 381 * :You are now an operator at " + it->getName() + "\r\n");
                            it->broadcastMsg(clientPrefix(client) + " MODE " + target + " +o " + opIt->getNickn() + "\r\n");
                            return;
                        }
                        if (modeFlag == "-o") {
                            it->removeOp(*opIt);
                            sendMsg(opIt->getFd(), ":ircserv 400 * :You are no longer an operator at " + it->getName() + "\r\n");
                            it->broadcastMsg(clientPrefix(client) + " MODE " + target + " -o " + opIt->getNickn() + "\r\n");
                            return;
                        }
                    }
                }
                sendMsg(client->getFd(), ":ircserv 401 * :No such nick\r\n");
                return;
            }    
            if (modeFlag == "+i") {
                it->setHasInviteOnly(true);
                it->broadcastMsg(clientPrefix(client) + " MODE " + target + " +i\r\n");
                return;
            }
            if (modeFlag == "+t") {
                it->setHasTopicProtection(true);
                it->broadcastMsg(clientPrefix(client) + " MODE " + target + " +t\r\n");
                return;
            }
            if (modeFlag == "+k") {
                it->setKey(modeStr);
                it->setHasKey(true);
                it->broadcastMsg(clientPrefix(client) + " MODE " + target + " +k " + modeStr + "\r\n");
                return;
            }
            if (modeFlag == "+l") {
                if (modeStr.find_first_not_of("0123456789") != string::npos) {
                    sendMsg(client->getFd(), ":ircserv 501 * :Limit has to be a number\r\n");
                    return;
                }
                it->setLimit(stringToSizeT(modeStr));
                it->setHasLimit(true);
                it->broadcastMsg(clientPrefix(client) + " MODE " + target + " +l " + modeStr + "\r\n");
                return;
            }
            if (modeFlag == "-i") {
                it->setHasInviteOnly(false);
                it->broadcastMsg(clientPrefix(client) + " MODE " + target + " -i\r\n");
                return;
            }
            if (modeFlag == "-t") {
                it->setHasTopicProtection(false);
                it->broadcastMsg(clientPrefix(client) + " MODE " + target + " -t\r\n");
                return;
            }
            if (modeFlag == "-k") {
                it->setKey("");
                it->setHasKey(false);
                it->broadcastMsg(clientPrefix(client) + " MODE " + target + " -k\r\n");
                return;
            }
            if (modeFlag == "-l") {
                it->setLimit(0);
                it->setHasLimit(false);
                it->broadcastMsg(clientPrefix(client) + " MODE " + target + " -l\r\n");
                return;
            }
            sendMsg(client->getFd(), ":ircserv 501 * :Unknown mode\r\n");
            return;
        }
    }
    sendMsg(client->getFd(), ":ircserv 403 * :No such channel\r\n");
    return;
}
