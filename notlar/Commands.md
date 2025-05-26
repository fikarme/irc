valgrind \
  --leak-check=full \
  --show-leak-kinds=all \
  --track-origins=yes \
  --verbose \
  ./ircserv 



**Welcome**
register sonraki ilk mesaj. networkname ör: Libera Chat bizde Lain olabilir
001 RPL_WELCOME   <client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]

ikinci mesaj. servername ör: london.libera.chat ama bizde ip olabilir
002 RPL_YOURHOST  <client> :Your host is <servername>, running version <version>"

ör: Sal May 25 18:33:09 2021
003 RPL_CREATED   <client> :This server was created <datetime>"

`PASS <password>`
PASS -> NICK -> USER -> WelcomeMsg
461 ERR_NEEDMOREPARAMS      Parametre eksikliğinde
462 ERR_ALREADYREGISTERED   Zaten kayıtlı kullanıcı için
464 ERR_PASSWDMISMATCH      Şifre eşleşmediğinde
USER/NICK komutlarıdan önce PASS alınmalı ama LiberaChat gibi eski freenode sunucular PASS gerektirmez.
Hoşgeldin mesajı PASS NICK USER tamamlandığında gönderilir

`KICK <channel> <user> [<comment>]`
451 ERR_NOTREGISTERED       bağlantı kontrolü
461 ERR_NEEDMOREPARAMS      Eksik parametre durumunda
476 ERR_BADCHANMASK         Geçersiz kanal adı için #sız
403 ERR_NOSUCHCHANNEL       Kanal bulunamadığında
441 ERR_USERNOTINCHANNEL    Atılacak kullanıcı kanalda değilse
442 ERR_NOTONCHANNEL        Komutu veren kanalda değilse
482 ERR_CHANOPRIVSNEEDED    Kullanıcı operator değilse
Kanalı terk etmesi gereken kullanıcıya ve kanaldaki diğer tüm kullanıcılara bildirim
Kendini Atma Senaryosu: operatör kendini atmaya çalışırsa, önce yeni bir operatör seçmesi isteniyor. bu ekstra
Kanal boş kaldıysa kanalın kapatılması

`USER <username> 0 * <realname>`
462 ERR_ALREADYREGISTERED   zaten kayıtlıysa reddet
461 ERR_NEEDMOREPARAMS      4 param
usernamede bşluk realnamede boşluk :
Ident Protokolü kullanılmıyorsa username önüne "~"
Username uzunluğu USERLEN sınırına göre kısaltılmalı
Boş username ise, nickname fallback olarak
NIC VE USER tamamlandığında connected

`INVITE <nickname> <channel>`
451 ERR_NOTREGISTERED       Bağlantı kontrolü
461 ERR_NEEDMOREPARAMS      Eksik parametre durumunda
476 ERR_BADCHANMASK         Geçersiz kanal adı için #'sız
401 ERR_NOSUCHNICK          Davet edilecek kullanıcı bulunamadığında
403 ERR_NOSUCHCHANNEL       Kanal bulunamadığında
442 ERR_NOTONCHANNEL        Komutu veren kanalda değilse
482 ERR_CHANOPRIVSNEEDED    Kullanıcı operator değilse (kanal inviteonlyse)
443 ERR_USERONCHANNEL       Davet edilen kullanıcı zaten kanalda ise
1.addInvited
2. invite msg
3. komutu verene 341 RPL_INVITING
4. invite edilen broadcast edilmemeli ama biz edelim
opsiyonel:boş INVITE kullanıldığında invite edildiğin kanal listesi

`NICK <nickname>`
451 ERR_NOTREGISTERED       modern serverlarda güvenlik nedeniyle ama bizde gerek yok oldschool takılcaz B)
431 ERR_NONICKNAMEGIVEN     Nickname parametresi eksikse
432 ERR_ERRONEUSNICKNAME    Geçersiz karakterler içeren nickname
new_nick.find(' ') != std::string::npos || new_nick[0] == '#' || new_nick[0] == ':'
433 ERR_NICKNAMEINUSE       Nickname zaten kullanımda
436 ERR_NICKCOLLISION       Nickname çakışması (çoklu sunucu ortamında)
1.nick güncellenir
2. değiştirene msg
3. herkese yeni nick broadcast
5. kanallardaki nick güncellenir

`TOPIC <channel> [<topic>]`
451 ERR_NOTREGISTERED       Bağlantı kontrolü
461 ERR_NEEDMOREPARAMS      Eksik parametre durumunda
476 ERR_BADCHANMASK         Geçersiz kanal adı için #'sız
403 ERR_NOSUCHCHANNEL       Kanal bulunamadığında
442 ERR_NOTONCHANNEL        Komutu veren kanalda değilse
482 ERR_CHANOPRIVSNEEDED    Topic koruması açıksa ve kullanıcı op değilse
    topiği göster
331 RPL_NOTOPIC             topic yok
332 RPL_TOPIC               topic var
333 RPL_TOPICWHOTIME        topici kim ne zaman koydu
1.param yoksa topic gösterilir
2. TOPIC #kanal :  ise topic temizlenir
4. değilse topiği değiştir herkesi notifyla
5. topic değişince who time kaydet

`MODE <target> [<modestring> [<mode arguments>...]]`
451 ERR_NOTREGISTERED       Bağlantı kontrolü
461 ERR_NEEDMOREPARAMS      Eksik parametre durumunda
    Kullanıcı için:
401 ERR_NOSUCHNICK          Hedef kullanıcı bulunamadığında
502 ERR_USERSDONTMATCH      Başka kullanıcının modunu değiştirme girişimi
501 ERR_UMODEUNKNOWNFLAG    Bilinmeyen kullanıcı modu
221 RPL_UMODEIS             Mevcut kullanıcı modlarını göster (modestring yoksa)
    Kanal için:
476 ERR_BADCHANMASK         Geçersiz kanal adı için #'sız
403 ERR_NOSUCHCHANNEL       Kanal bulunamadığında
442 ERR_NOTONCHANNEL Komutu veren kanalda değilse
482 ERR_CHANOPRIVSNEEDED    Kullanıcı operator değilse
324 RPL_CHANNELMODEIS       Kanal modlarını göster (modestring yoksa)
329 RPL_CREATIONTIME        Kanal oluşturma zamanını göster

`JOIN <channel> <key>`
451 ERR_NOTREGISTERED       Bağlantı kontrolü
461 ERR_NEEDMOREPARAMS      Eksik parametre durumunda
476 ERR_BADCHANMASK         Geçersiz kanal adı
403 ERR_NOSUCHCHANNEL       Kanal yoksa
405 ERR_TOOMANYCHANNELS     Kullanıcı limit aşımı
475 ERR_BADCHANNELKEY       Yanlış anahtar
473 ERR_INVITEONLYCHAN      Invite-only ve davet edilmemişse
471 ERR_CHANNELISFULL       Kanal doluysa
1.kullanıcının katıldığını broadcast
2. 332 331 topic gönder
3. 353 366 kullanıcı listesini gönder
4. ilk katılan kullanıcıyı op
5. kanal modlarını ayarla key vs
JOIN 0 tüm kanallardan ayrılma
virgülle çoklu kanala katılma (opsiyonel)

`PRIVMSG <target>{,<target>} <text to be sent>`
451 ERR_NOTREGISTERED       Bağlantı kontrolü
461 ERR_NEEDMOREPARAMS      Eksik parametre durumunda
412 ERR_NOTEXTTOSEND        Gönderilecek metin yoksa
407 ERR_TOOMANYTARGETS      Çok fazla hedef varsa
    kanal için:
403 ERR_NOSUCHCHANNEL       Kanal yoksa
442 ERR_NOTONCHANNEL        Kullanıcı kanalda değilse
    kullanıcı için:
401 ERR_NOSUCHNICK          Kullanıcı yoksa
