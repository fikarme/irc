- This project contains!: Understanding bi-directional file-descriptor communication (aka sockets).
- IRC is a simple and robust protocol built to host communities.
- IRC is open to anyone. You can create a server or build a client.
- IRC has four fundamental pillars: server, command parser, channels and the client(not the actual client that u will use).
- """Clean code (e.g. all messages are created with the builder design pattern?)"""

# Server
- managing socket connections
- safeguarding variables like server password
- realtime interactions with IRC network <-> clients

# Parser
- handle a global authentication wall
- make sure that most commands are not accessible to non-registered users
- UNAUTHENTICATED -> AUTHENTICATED -> REGISTERED

# Channels
- each has its own rules and vibes ;]
- has to prefix# (#general)
- it can has open, invite, password
- track a list of members
- manage operator privileges like topic, operators, limit, kick
- synchronisation: ensure that msg is broadcasted to all members
- some settings upon init some settings with MODE

# Clients
- its not a person its connection to server
- handles nickname, username, hostname, realname, FD, auth state
- track which channels the user is in
- handle in/out msg
- ensure that the msg is formatted correctly
- PRVMSG?

# Yorum
- Soket yönetimi bir dinleme noktası oluşturup bağlayıp(binding)  dinlemekle başlar.
- Bu soket istemcilerden gelen bağlantı isteklerini dinler.
- Bağlantı kurulduktan sonra, sunucu istemciyle iletişim kurmak için başka bir soket kullanır.
- Soketler şöyle yönetilir: soketi oluşturmak, bir bağlantı noktasına bağlamak, gelen bağlantıları dinlemek ve bu bağlantıları kabul etmek.
- Kabul edilen her bağlantı daha sonra yeni bir soket tarafından ele alınır ve birden fazla istemcinin sunucuyla eşzamanlı olarak etkileşime girmesine olanak tanır.

- RFC yerine modern horse ya da simple wiki
- KVIRC ya da Halloy istemcisi
- her olay doğru istemcilere gösterilmeli join/leave/kick/mode gibi
- kullanıcının tüm isimleri farklı ya da aynı olursa?
- +k
- @ is it meaning op?


# Linkler
https://www.unrealircd.org/

https://www.inspircd.org/

liberachat ##turkey

https://www.ircrehberi.net/

----

https://www.tutorialspoint.com/unix_sockets/what_is_socket.htm

https://www.tutorialspoint.com/unix_sockets/what_is_socket.htm

https://www.tutorialspoint.com/unix_sockets/socket_core_functions.

https://beej.us/guide/bgnet/html/

----

https://simple.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands

https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/

https://ircv3.net/irc/

https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9

https://medium.com/@talhadmr/ft-irc-server-92ffcd1d4338

https://modern.ircdocs.horse/

https://datatracker.ietf.org/doc/html/rfc2812

https://chi.cs.uchicago.edu/chirc/index.html

https://chi.cs.uchicago.edu/chirc/irc.html

https://chi.cs.uchicago.edu/chirc/irc_examples.html#chirc-irc-examples

https://yeslee-v.medium.com/internet-relay-chat-irc-guide-7bd418a73e6a

https://yeslee-v.medium.com/internet-relay-chat-irc-guide-2-internet-relay-chat-7415dca58bad

https://yeslee-v.medium.com/internet-relay-chat-irc-guide-3-example-irc-communications-202c9e8397ad

https://glitter-muskox-81c.notion.site/KVIrc-IRC-Projesi-Client-a1c59174db354bb4900ac76a8d7435d2

-----

https://github.com/Link-Wolf/ft_irc

https://github.com/Talhadmr/ft_irc

https://github.com/42YerevanProjects/ft_irc

https://github.com/abdelbenamara/FtIRC

https://github.com/AhmedFatir/ft_irc

https://github.com/Alexdelia/42-ft_irc

https://github.com/annafenzl/42-ft_irc

https://github.com/barimehdi77/ft_irc

https://github.com/jsoulet42/ft_irc
