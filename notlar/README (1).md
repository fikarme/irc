## Komutlar

Syntax|Açıklama|OP
-|-|-
`USER <username> <hostname> <servername> <realname>` | İlk bağlantıda zorunlu kullanıcı kaydı
`PASS <password>` | *** 
`NICK <nickname>` | Benzersiz takma isim 
`JOIN <channel name> <password>` | Kanala katılma
`PRIVMSG <nickname> <message>` | Özel mesaj gönderme
`MODE <channel name> <flags>` | o:peratör k:ey i:nvite t:opic l:imit
`HELP <command>` | Komutları listeme
`TOPIC <channel name> <topic>` | Kanalın konusunu değiştirme |<ul><li>- [x] </li>
`KICK <channel name> <nickname> <reason>` | Kullanıcıyı kanaldan atma |<ul><li>- [x] </li>
`INVITE <nickname> <channel>` | Kanala davet etme |<ul><li>- [x] </li>

## Kullanılabilecek Harici Fonksiyonlar

- [x] `socket`
- [x] `close`
- [x] `setsockopt`
- `getsockname`
- `getprotobyname`
- `gethostbyname`
- `getaddrinfo`
- `freeaddrinfo`
- [x] `bind`
- `connect`
- [x] `listen`
- [x] `accept`
- [x] `htons`
- `htonl`
- `ntohs`
- `ntohl`
- [x] `inet_addr`
- `inet_ntoa`
- [x] `send`
- [x] `recv`
- `signal`
- `sigaction`
- `lseek`
- `fstat`
- [x] `fcntl`
- [x] `poll` (veya eşdeğeri)

## Kullanılan Harici Fonksiyonlar ve Argümanları

### `socket`
  - `int domain`: Adres ailesi (ör: `AF_INET` IPv4 için).
  - `int type`: Soket türü (ör: `SOCK_STREAM` TCP için).
  - `int protocol`: Protokol numarası (genellikle `0`).

### `setsockopt`
  - `int sockfd`: Soket dosya tanımlayıcısı.
  - `int level`: Seviye (ör: `SOL_SOCKET`).
  - `int optname`: Seçenek adı (ör: `SO_REUSEADDR`).
  - `const void *optval`: Seçenek değeri.
  - `socklen_t optlen`: Seçenek değeri uzunluğu.

### `bind`
  - `int sockfd`: Soket dosya tanımlayıcısı.
  - `const struct sockaddr *addr`: Adres yapısı.
  - `socklen_t addrlen`: Adres yapısı uzunluğu.

### `listen`
  - `int sockfd`: Soket dosya tanımlayıcısı.
  - `int backlog`: Kuyruk uzunluğu.

### `accept`
  - `int sockfd`: Soket dosya tanımlayıcısı.
  - `struct sockaddr *addr`: Bağlanan istemcinin adresi.
  - `socklen_t *addrlen`: Adres yapısı uzunluğu.

### `htons`
  - `uint16_t hostshort`: Host byte sırasındaki kısa tamsayı.

### `inet_addr`
  - `const char *cp`: IP adresi stringi.

### `send`
  - `int sockfd`: Soket dosya tanımlayıcısı.
  - `const void *buf`: Gönderilecek veri.
  - `size_t len`: Veri uzunluğu.
  - `int flags`: Bayraklar (genellikle `0`).

### `recv`
  - `int sockfd`: Soket dosya tanımlayıcısı.
  - `void *buf`: Alınacak veri için tampon.
  - `size_t len`: Tampon uzunluğu.
  - `int flags`: Bayraklar (genellikle `0`).

### `close`
  - `int fd`: Dosya tanımlayıcısı.

### `fcntl`
  - `int fd`: Dosya tanımlayıcısı.
  - `int cmd`: Komut (ör: `F_SETFL`).
  - `...`: Ek argümanlar (ör: `O_NONBLOCK`).

### `poll`
  - `struct pollfd *fds`: İzlenecek dosya tanımlayıcıları dizisi.
  - `nfds_t nfds`: Dizideki dosya tanımlayıcılarının sayısı.
  - `int timeout`: Zaman aşımı süresi (milisaniye cinsinden).