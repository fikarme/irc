#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Utils.hpp"

class Channel {
	private:
		string _name;
		string _key;
		string _topic;
		vector<Client> _clients;
		vector<Client> _ops;
		vector<Client> _invited;

		size_t _limit;
		bool _hasLimit;
		bool _hasKey;
		bool _hasTopicProtection;
		bool _hasInviteOnly;

	public:
		~Channel();
		Channel(const Channel &rhs);
		Channel(const string name, string key);
		Channel &operator=(const Channel &rhs);

		void setName(const string name);
		string getName() const;

		void setTopic(const string topic);
		string getTopic() const;

		void addClient(Client client);
		void removeClient(Client client);
		vector<Client>& getClients();

		void addOp(Client client);
		void removeOp(Client client);
		bool isOp(Client client) const;
		vector<Client> getOps() const;

		void setKey(string key);
		string getKey() const;

		void setLimit(size_t limit);
		size_t getLimit() const;

		void setHasLimit(bool hasLimit);
		bool getHasLimit() const;

		void setHasKey(bool hasKey);
		bool getHasKey() const;

		void setHasTopicProtection(bool hasTopicProtection);
		bool getHasTopicProtection() const;

		void setHasInviteOnly(bool hasInviteOnly);
		bool getHasInviteOnly() const;

		void addInvited(Client client);
		void removeInvited(Client client);
		bool isInvited(Client client) const;

		bool isIn(const vector<Client> &clientList, const Client &client) const;
		bool isInChannel(const string nick) const;

		Client getClientByNick(const string nick) const;
		void updateNickn(const string &oldNickn, const string &newNickn);

		void broadcastMsg(const string &msg);
		void removeFromVector(vector<Client>& vec, const Client& client);
		string getMode() const;
		
		void printChannel() const;
};

#endif
